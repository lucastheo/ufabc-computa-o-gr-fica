#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget(){
    destroyVBOs(model);
    destroyShaders(model);
}

void OpenGLWidget::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(1,1,1,1);
    qDebug("OpenGl Version> %s", glGetString(GL_VERSION));
    qDebug("OpenGl Version> %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST); // abilita a verificação se as coisas estão na frente ou atras
}

void OpenGLWidget::resizeGL(int w, int h){}

void OpenGLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
void OpenGLWidget::toggleDarkMode(bool var ){
    makeCurrent();
    if( var ) glClearColor(0,0,0,1);
    else glClearColor(1,1,1,1);
    update();
}

void OpenGLWidget::destroyVBOs(std::shared_ptr<Model>){}
void OpenGLWidget::destroyShaders(std::shared_ptr<Model>){}
void OpenGLWidget::createVBOs(std::shared_ptr<Model>){}
void OpenGLWidget::createShaders(std::shared_ptr<Model> m){
    makeCurrent();
    if(!m){ qDebug("M é vazio"); return; }
    destroyShaders(m);

    for( size_t i{0} ; i< m->shaderProgram.size(); i++ ){
        QFile vs(m->vertexShaderFileName[i]);
        QFile fs(m->fragmentShaderFileName[i]);
        if( vs.open(QFile::ReadOnly | QFile::Text)){
            qDebug("Vertex shader not found");
            vs.close(); fs.close();
            return;
        }
        if( fs.open(QFile::ReadOnly | QFile::Text)){
            qDebug("Fragment shader not found");
            vs.close(); fs.close();
            return;
        }
        auto byteArrayVs{vs.readAll()};
        auto byteArrayFs{fs.readAll()};

        const char *c_strVs{byteArrayVs};
        const char *c_strFs{byteArrayFs};
        vs.close(); fs.close();

        GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
        glShaderSource(vertexShader,1,&c_strVs,0);
        glCompileShader(vertexShader);
        GLuint isCompiled{0};
        glGetShaderiv( vertexShader , GL_COMPILE_STATUS, &isCompiled );

    }

}

void OpenGLWidget::showFileOpenDialog(){
    QString fileName = QFileDialog::getOpenFileName(this , "Open File" , QDir::homePath(), QString("Off File(*.off)"),nullptr
                                                                                               #ifdef Q_OS_LINUX
                                                                                                    , QFileDialog::DontUseNativeDialog
                                                                                               #endif
                                                                                                   );
    if(!fileName.isEmpty()){
        model = std::make_shared<Model>();
        model->readOFFFile(fileName);

        createShaders(model);
        createVBOs(model);
    }
    emit statusBarMessage(QString("Vertices: %1 Faces: %2").arg(model->numVertives).arg(model->numVertives));
    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
    break;
    }
}

