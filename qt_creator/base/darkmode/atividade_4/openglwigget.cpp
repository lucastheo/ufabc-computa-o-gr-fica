#include "openglwigget.h"

OpenGLWigget::OpenGLWigget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void OpenGLWigget::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0,1,1,1);
    QObject::connect( &timer , &QTimer::timeout ,
                      [&](){
                        blinkFlag = !blinkFlag;
                        this->update();
                                             } );
    timer.start(1000);
}

void OpenGLWigget::resizeGL(int w, int h){

}

// update chama isso aqui
void OpenGLWigget::paintGL(){
    modificarCorDoFundo( this->blinkFlag);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWigget::modificarCorDoFundo( bool var ){
    makeCurrent();
        if( var ) glClearColor(1,1,1,1);
        else      glClearColor(0,0,0,1);
    update();
}
