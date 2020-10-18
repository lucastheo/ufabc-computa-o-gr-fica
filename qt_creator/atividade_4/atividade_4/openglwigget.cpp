#include "openglwigget.h"

OpenGLWigget::OpenGLWigget(QWidget *parent) : QOpenGLWidget(parent)
{

}
OpenGLWigget::~OpenGLWigget() {

}


void OpenGLWigget::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0,1,1,1);
    /*QObject::connect( &timer , &QTimer::timeout ,
                      [&](){
                        this->blinkFlag = !this->blinkFlag;
                        this->update();
                                             } );
    timer.start(1000);
    */

    qDebug("Open GL Version %s" , glGetString((GL_VERSION)));
    qDebug("Open GL Version %s" , glGetString((GL_SHADING_LANGUAGE_VERSION)));
    this->createShaders();
    this->createVBOs();

}

void OpenGLWigget::resizeGL(int w, int h){

}

// update chama isso aqui
void OpenGLWigget::paintGL(){
    //modificarCorDoFundo( this->blinkFlag);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram( sharderProgram );
    glBindVertexArray(vao1);
    glDrawElements(GL_POLYGON, indices1.size() , GL_UNSIGNED_INT , 0 );

    glUseProgram( sharderProgram );
    glBindVertexArray(vao2);
    glDrawElements(GL_LINE_LOOP, indices2.size() , GL_UNSIGNED_INT , 0 );

    glUseProgram( sharderProgram );
    glBindVertexArray(vao3);
    glDrawElements(GL_POLYGON, indices3.size() , GL_UNSIGNED_INT , 0 );
}

void OpenGLWigget::modificarCorDoFundo( bool var ){
    makeCurrent();
        if( var ) glClearColor(1,1,0,1);
        else      glClearColor(0.75,0.75,0.75,1);
    update();
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
    if( resultadoCompilacaoShader( vertexShader ) ){
        glDeleteShader(vertexShader);
    }

    GLuint fragementShader = criandoShader(":/shaders/fshader1.glsl" , GL_FRAGMENT_SHADER );
    if( resultadoCompilacaoShader( fragementShader ) == false){
        glDeleteShader(fragementShader);
        glDeleteShader(vertexShader);
    }

    this->sharderProgram = glCreateProgram();
    glAttachShader(this->sharderProgram,vertexShader);
    glAttachShader(this->sharderProgram,fragementShader);
    glLinkProgram(this->sharderProgram);

    GLint isLinked{0};
    glGetProgramiv(this->sharderProgram, GL_LINK_STATUS ,&isLinked);
    if( resultadoCompilacaoPrograma( this->sharderProgram ) == false){
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

    vertices1.resize(2000);
    colors1.resize(vertices1.size());
    indices1.resize(vertices1.size()*2);
    int verticesMetade = (vertices1.size())/2 - 1;
    float solTamanho{1};
    float solPosicaoX{-0.5};
    float solPosicaoY = 0;
    for( GLuint i = 0; i <= verticesMetade; i++ ){
        vertices1[ i                  ]     = QVector4D(    ( solPosicaoX + float(i    )/verticesMetade )*solTamanho     ,(solPosicaoY + pow(1 - pow( ( float(i    )/(verticesMetade))*2 -1 , 2 ) , 0.5) )*solTamanho*0.5,0,1);
        vertices1[ i + verticesMetade + 1 ] = QVector4D( ( ( solPosicaoX + float(i + 1)/verticesMetade )*solTamanho )*-1 ,(solPosicaoY - pow(1 - pow( ( float(i + 1)/(verticesMetade))*2 -1 , 2 ) , 0.5)) *solTamanho*0.5,0,1);
    }
    for( GLuint i = 0; i < vertices1.size(); i++ ){
        colors1[ i ] = QVector4D(1,0,1,1);
    }
    for( GLuint i = 0; i < vertices1.size(); i++ ){
        indices1[ i*2    ] = i;
        indices1[ i*2 + 1] = i+1;
    }

    glGenVertexArrays( 1 ,&vao1 );
    glBindVertexArray(vao1);

    glGenBuffers(1,&vboVertices1);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertices1);
    glBufferData(GL_ARRAY_BUFFER,vertices1.size()*sizeof(QVector4D), vertices1.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&vboColors1);
    glBindBuffer(GL_ARRAY_BUFFER,vboColors1);
    glBufferData(GL_ARRAY_BUFFER,colors1.size()*sizeof(QVector4D), colors1.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1 , &eboIndices1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboIndices1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices1.size()*sizeof(GLuint), indices1.data(), GL_STATIC_DRAW);

    vertices2.resize(12);
    colors2.resize(vertices2.size());
    indices2.resize(vertices2.size()*2 + 2);
    vertices2[ 0 ] = QVector4D(-0.95,-0.85,1,1);
    vertices2[ 1 ] = QVector4D(-0.70,-0.85,1,1);
    vertices2[ 2 ] = QVector4D(-0.70,-0.90,1,1);
    vertices2[ 3 ] = QVector4D(-0.75,-0.90,1,1);
    vertices2[ 4 ] = QVector4D(-0.75,-0.95,1,1);
    vertices2[ 5 ] = QVector4D(-0.80,-0.95,1,1);
    vertices2[ 6 ] = QVector4D(-0.80,-0.90,1,1);
    vertices2[ 7 ] = QVector4D(-0.85,-0.90,1,1);
    vertices2[ 8 ] = QVector4D(-0.85,-0.95,1,1);
    vertices2[ 9 ] = QVector4D(-0.90,-0.95,1,1);
    vertices2[10 ] = QVector4D(-0.90,-0.90,1,1);
    vertices2[11 ] = QVector4D(-0.95,-0.90,1,1);

    for( GLuint i = 0; i < vertices2.size(); i++ ){
        colors2[ i ] = QVector4D(1,0,0,1);
    }
    indices2[ 0 ] = 11;
    indices2[ 1 ] = 0;
    for( GLuint i = 1; i < vertices2.size() + 1; i++ ){
        indices2[ i*2    ] = i;
        indices2[ i*2 + 1] = i+1;
    }
    glGenVertexArrays( 1 ,&vao2 );
    glBindVertexArray(vao2);

    glGenBuffers(1,&vboVertices2);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertices2);
    glBufferData(GL_ARRAY_BUFFER,vertices2.size()*sizeof(QVector4D), vertices2.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&vboColors2);
    glBindBuffer(GL_ARRAY_BUFFER,vboColors2);
    glBufferData(GL_ARRAY_BUFFER,colors2.size()*sizeof(QVector4D), colors2.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1 , &eboIndices2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboIndices2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices2.size()*sizeof(GLuint), indices2.data(), GL_STATIC_DRAW);



    vertices3.resize(8);
    colors3.resize(vertices2.size());
    indices3.resize(vertices2.size()*2 + 2);
    vertices3[ 0 ] = QVector4D( 0.80,-0.70,1,1);
    vertices3[ 1 ] = QVector4D( 0.82,-0.78,1,1);
    vertices3[ 2 ] = QVector4D( 0.90,-0.80,1,1);
    vertices3[ 3 ] = QVector4D( 0.82,-0.82,1,1);
    vertices3[ 4 ] = QVector4D( 0.80,-0.90,1,1);
    vertices3[ 5 ] = QVector4D( 0.78,-0.82,1,1);
    vertices3[ 6 ] = QVector4D( 0.70,-0.80,1,1);
    vertices3[ 7 ] = QVector4D( 0.78,-0.78,1,1);

    for( GLuint i = 0; i < vertices3.size(); i++ ){
        colors3[ i ] = QVector4D(0,1,0,1);
    }
    indices3[ 0 ] = 7;
    indices3[ 1 ] = 0;
    for( GLuint i = 1; i < vertices3.size() + 1; i++ ){
        indices3[ i*2    ] = i;
        indices3[ i*2 + 1] = i+1;
    }
    glGenVertexArrays( 1 ,&vao3 );
    glBindVertexArray(vao3);

    glGenBuffers(1,&vboVertices3);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertices3);
    glBufferData(GL_ARRAY_BUFFER,vertices3.size()*sizeof(QVector4D), vertices3.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&vboColors3);
    glBindBuffer(GL_ARRAY_BUFFER,vboColors3);
    glBufferData(GL_ARRAY_BUFFER,colors3.size()*sizeof(QVector4D), colors3.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT ,GL_FALSE, 0 , nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1 , &eboIndices3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboIndices3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices3.size()*sizeof(GLuint), indices3.data(), GL_STATIC_DRAW);

}

void OpenGLWigget::destroyVBOs(){
    makeCurrent();
    glDeleteBuffers( 1 , &vboVertices1);
    glDeleteBuffers( 1 , &vboColors1);
    glDeleteBuffers( 1 , &eboIndices1);
    glDeleteBuffers( 1 , &vao1);
    vboVertices1 = 0;
    vboColors1 = 0;
    eboIndices1 = 0;
    vao1 = 0;

    glDeleteBuffers( 1 , &vboVertices2);
    glDeleteBuffers( 1 , &vboColors2);
    glDeleteBuffers( 1 , &eboIndices2);
    glDeleteBuffers( 1 , &vao2);
    vboVertices2 = 0;
    vboColors2 = 0;
    eboIndices2 = 0;
    vao2 = 0;

    glDeleteBuffers( 1 , &vboVertices3);
    glDeleteBuffers( 1 , &vboColors3);
    glDeleteBuffers( 1 , &eboIndices3);
    glDeleteBuffers( 1 , &vao1);
    vboVertices3 = 0;
    vboColors3 = 0;
    eboIndices3 = 0;
    vao3 = 0;
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
