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

    if(!model) return;

    auto shaderProgramId{model->shaderProgram[model->currentShader]};
    glUseProgram(shaderProgramId);

    glBindVertexArray(model->vao);
    glDrawElements(GL_TRIANGLES , model->indices.size(),GL_UNSIGNED_INT, nullptr);
}

void OpenGLWidget::toggleDarkMode(bool var ){
    makeCurrent();
    if( var ) glClearColor(0,0,0,1);
    else glClearColor(1,1,1,1);
    update();
}

void OpenGLWidget::destroyVBOs(std::shared_ptr<Model> m){
    makeCurrent();
    if(!m) return;
    glDeleteBuffers(1,&m->vboVertice);
    glDeleteBuffers(1,&m->eboIndices);
    glDeleteVertexArrays(1,&m->vao);
    m->vboVertice=0;
    m->eboIndices=0;
    m->vao=0;
}

void OpenGLWidget::destroyShaders(std::shared_ptr<Model> m){
    makeCurrent();
    if(!m) return;
    for(GLuint shaderProgramID :m->shaderProgram){
        glDeleteProgram((shaderProgramID));
    }
}

void OpenGLWidget::createVBOs(std::shared_ptr<Model> m){
    makeCurrent();
    if(!m) return;

    destroyVBOs(m);
    glGenVertexArrays( 1 , &m->vao);
    glBindVertexArray(m->vao);

    glGenBuffers(1,&m->vboVertice);
    glBindBuffer(GL_ARRAY_BUFFER,m->vboVertice);
    glBufferData(GL_ARRAY_BUFFER,m->numVertives*sizeof (QVector4D),m->vertices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1 ,&m->eboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m->eboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numFaces*3*sizeof (GLuint),m->indices.data(),GL_STATIC_DRAW);
}

void OpenGLWidget::createShaders(std::shared_ptr<Model> m){
    makeCurrent();
    if(!m){ qDebug("M é vazio"); return; }
    destroyShaders(m);

    for( size_t i{0} ; i< m->shaderProgram.size(); i++ ){
        QFile vs(m->vertexShaderFileName[i]);
        QFile fs(m->fragmentShaderFileName[i]);
        if( !vs.open(QFile::ReadOnly | QFile::Text)){
            qDebug("Vertex shader not found");
            vs.close(); fs.close();
            return;
        }
        if( !fs.open(QFile::ReadOnly | QFile::Text)){
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
        GLint isCompiled{0};

        glGetShaderiv( vertexShader , GL_COMPILE_STATUS, &isCompiled );
        if( !isCompiled ){
            GLint maxLength{0};
            glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength,&maxLength,&infoLog[0]);
            qDebug("%s", &infoLog[0]);
            glDeleteShader(vertexShader);
            return;
        }

        GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
        glShaderSource(fragmentShader,1,&c_strFs,0);
        glCompileShader(fragmentShader);
        isCompiled = {0};

        glGetShaderiv( fragmentShader , GL_COMPILE_STATUS, &isCompiled );
        if( !isCompiled ){
            GLint maxLength{0};
            glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength,&maxLength,&infoLog[0]);
            qDebug("%s", &infoLog[0]);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return;
        }

        m->shaderProgram[i] = glCreateProgram();
        glAttachShader(m->shaderProgram[i],vertexShader);
        glAttachShader(m->shaderProgram[i],fragmentShader);
        glLinkProgram(m->shaderProgram[i]);

        GLint isLinked{0};
        glGetProgramiv(m->shaderProgram[i] , GL_LINK_STATUS , &isLinked);
        if( !isLinked ){
            GLint maxLength{0};
            glGetProgramiv(fragmentShader,GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(fragmentShader, maxLength,&maxLength,&infoLog[0]);
            qDebug("%s", &infoLog[0]);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(m->shaderProgram[i]);
            return;
        }

        glDetachShader(m->shaderProgram[i],vertexShader);
        glDetachShader(m->shaderProgram[i],fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


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

