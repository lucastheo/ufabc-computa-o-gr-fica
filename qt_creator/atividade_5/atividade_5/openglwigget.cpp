#include "openglwigget.h"

OpenGLWigget::OpenGLWigget(QWidget *parent) : QOpenGLWidget(parent)
{

}
OpenGLWigget::~OpenGLWigget() {

}


void OpenGLWigget::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
    QObject::connect( &timer , &QTimer::timeout ,
                      &OpenGLWigget::animate );
    timer.start(10);


    qDebug("Open GL Version %s" , glGetString((GL_VERSION)));
    qDebug("Open GL Version %s" , glGetString((GL_SHADING_LANGUAGE_VERSION)));
    this->createShaders();
    this->createVBOs();

}

void OpenGLWigget::resizeGL(int w, int h){

}

void OpenGLWigget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram( sharderProgram );
    GLint locScaling{glGetUniformLocation( sharderProgram , "scaling")};
    GLint locTranslation{glGetUniformLocation(sharderProgram, "translation")};
    glBindVertexArray(vao);

    //player
    glUniform4f(locTranslation , -0.8f , playerPosY , 0 , 0 );
    glUniform1f(locScaling , 0.2f);
    glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT , 0 );

    glUniform4f(locTranslation ,  0.8f , targetPosY , 0 , 0 );
    glUniform1f(locScaling , 0.2f);
    glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT , 0 );


}

GLuint OpenGLWigget::criandoShader( char  *c , GLenum type){
    QFile vs(c);
    if( !vs.open(QFile::ReadOnly | QFile::Text)){ qDebug("%s" , "Erro em abrir o vshaders"); return 0; }
    QByteArray byteArrayVs{vs.readAll()};
    const char *c_strVs{byteArrayVs};
    vs.close();

    GLuint shaderIndice{glCreateShader(type)};
    glShaderSource( shaderIndice , 1 , &c_strVs , 0 ); // 1 tamanho dos vertex e 0 o tamanho, 0 é tudo
    glCompileShader( shaderIndice );

    return shaderIndice;
}

void OpenGLWigget::createShaders(){
    makeCurrent();
    destroyShaders();
    GLuint vertexShader = criandoShader(":/shaders/vshader1.glsl" , GL_VERTEX_SHADER );
    if( !resultadoCompilacaoShader( vertexShader ) ){
        glDeleteShader(vertexShader);
        qDebug("Erro em criar o Vertex Shader");
        return;
    }

    GLuint fragementShader = criandoShader(":/shaders/fshader1.glsl" , GL_FRAGMENT_SHADER );
    if( !resultadoCompilacaoShader( fragementShader )){
        glDeleteShader(fragementShader);
        glDeleteShader(vertexShader);
        qDebug("Erro em criar o Fragement Shader");
        return;
    }

    this->sharderProgram = glCreateProgram();
    glAttachShader(this->sharderProgram,vertexShader);
    glAttachShader(this->sharderProgram,fragementShader);
    glLinkProgram(this->sharderProgram);

    GLint isLinked{0};
    glGetProgramiv(this->sharderProgram, GL_LINK_STATUS ,&isLinked);
    if( !resultadoCompilacaoPrograma( this->sharderProgram ) ){
        glDeleteShader(fragementShader);
        glDeleteShader(vertexShader);
        glDeleteProgram(this->sharderProgram);
        return;
    }

    glDetachShader(this->sharderProgram,vertexShader);
    glDetachShader(this->sharderProgram,fragementShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragementShader);
}


void OpenGLWigget::destroyShaders(){
    makeCurrent();
    glDeleteProgram(sharderProgram);
}

void OpenGLWigget::createVBOs(){
    makeCurrent();
    destroyVBOs();

    vertices.resize(4);
    colors.resize(4);
    indices.resize(6);
    vertices[ 0 ] = QVector4D(  0.5,  0.5 ,1,1);
    vertices[ 1 ] = QVector4D( -0.5,  0.5 ,1,1);
    vertices[ 2 ] = QVector4D( -0.5, -0.5 ,1,1);
    vertices[ 3 ] = QVector4D(  0.5, -0.5 ,1,1);

    colors[ 0 ] = QVector4D( 0 , 0 , 1 , 1 );
    colors[ 1 ] = QVector4D( 0 , 1 , 1 , 1 );
    colors[ 2 ] = QVector4D( 1 , 1 , 0 , 1 );
    colors[ 3 ] = QVector4D( 1 , 0 , 0 , 1 );


    indices[0] = 0;indices[1] = 1;indices[2] = 2;
    indices[3] = 0;indices[4] = 2;indices[5] = 3;

    glGenVertexArrays( 1 ,&vao );
    glBindVertexArray(vao);

    glGenBuffers(1,&vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertices);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(QVector4D), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&vboColors);
    glBindBuffer(GL_ARRAY_BUFFER,vboColors);
    glBufferData(GL_ARRAY_BUFFER,colors.size()*sizeof(QVector4D), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1 , &eboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);


}

void OpenGLWigget::destroyVBOs(){
    makeCurrent();
    glDeleteBuffers( 1 , &vboVertices);
    glDeleteBuffers( 1 , &vboColors);
    glDeleteBuffers( 1 , &eboIndices);
    glDeleteBuffers( 1 , &vao);
    vboVertices = 0;
    vboColors = 0;
    eboIndices = 0;
    vao = 0;
}


bool OpenGLWigget::resultadoCompilacaoShader( GLuint var ){

    GLint isCompiled{0};
    glGetShaderiv(var,GL_COMPILE_STATUS , &isCompiled );
    if( isCompiled == GL_FALSE ){
        GLint maxLenght{0};
        glGetShaderiv(var,GL_INFO_LOG_LENGTH,&maxLenght);
        std::vector<GLchar> infoLog( maxLenght);
        glGetShaderInfoLog(var, maxLenght, &maxLenght, &infoLog[0]);
        qDebug("Erro ao tentar compilar shader %s", &infoLog[0]);
        return false;
    }

    return true;
}

bool OpenGLWigget::resultadoCompilacaoPrograma( GLuint var ){
    GLint isLinked{0};
    glGetProgramiv(var, GL_LINK_STATUS ,&isLinked);
    if( isLinked==GL_FALSE){
        GLint maxLenght{0};
        glGetProgramiv(var, GL_INFO_LOG_LENGTH , &maxLenght );
        std::vector<GLchar> infoLog(maxLenght);
        glGetProgramInfoLog(var , maxLenght,&maxLenght, &infoLog[0] );
        qDebug("Programa %s", &infoLog[0]);
        return false;
    }
    return true;
}

void OpenGLWigget::animate(){
    float elTime{this->elapsedTimer.restart()/1000.0f};

    this->playerPosY += this->playerPosYOffset*elTime;

    if( targetPosY > .8f){
        targetPosY =.8f;
        targetPosYOffset *= -1;
    }else if( targetPosY < .8f){
        targetPosY =-.8f;
        targetPosYOffset *= -1;
    }

}
