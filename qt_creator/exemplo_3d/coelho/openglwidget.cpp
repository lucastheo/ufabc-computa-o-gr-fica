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
void OpenGLWidget::createShaders(std::shared_ptr<Model>){}
void OpenGLWidget::showFileOpenDialog(){}
void OpenGLWidget::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
    break;
    }
}

