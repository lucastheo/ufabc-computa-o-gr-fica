#include "openglwigget.h"

OpenGLWigget::OpenGLWigget(QWidget *parent) : QOpenGLWidget(parent)
{

}
OpenGLWigget::~OpenGLWigget() {

}


void OpenGLWigget::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
    QObject::connect( &timer , &QTimer::timeout , this , &OpenGLWigget::animate );
    this->timer.start(5);
    this->elapsedTimer.start();


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
    GLint locColor1{glGetUniformLocation(sharderProgram, "color1")};
    GLint locColor2{glGetUniformLocation(sharderProgram, "color2")};
    GLint locColor3{glGetUniformLocation(sharderProgram, "color3")};

    glBindVertexArray(vao);

    //player
    glUniform4f(locTranslation , -0.8f , playerPosY , 0 , 0 );
    glUniform1f(locScaling , 0.2f);
    glUniform1f(locColor1 , 1);
    glUniform1f(locColor2 , 1);
    glUniform1f(locColor3 , 1);
    glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT , 0 );

    glUniform4f(locTranslation ,  this->target[_X_POS_], this->target[_Y_POS_] , 0 , 0 );
    glUniform1f(locScaling , this->target[_SIZE_]);
    glUniform1f(locColor1 , this->target[_COLOR_]);
    glUniform1f(locColor2 , 1-this->target[_COLOR_]);
    glUniform1f(locColor3 , 0.5f);
    glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT , 0 );

    for( auto i = 0; i < this->projectiles.size() ; i++ ){
        glUniform4f(locTranslation ,  this->projectiles[i][_X_POS_] , this->projectiles[i][_Y_POS_] , 0 , 0 );
        glUniform1f(locScaling , this->projectiles[i][_SIZE_]);
        if( this->projectiles[i][_TYPE_] < TYPE_PROJECTILES_BASE + 0.1 ){
            glUniform1f(locColor1 , 0.5f);
            glUniform1f(locColor2 , 0.5f);
            glUniform1f(locColor3 , 0.0f);
        }
        else if( this->projectiles[i][_TYPE_] < TYPE_PROJECTILES_DUAL + 0.1){
            glUniform1f(locColor1 , 0);
            glUniform1f(locColor2 , 0);
            glUniform1f(locColor3 , 1);
        }else if( this->projectiles[i][_TYPE_] < TYPE_PROJECTILES_LATERAL + 0.1){
            glUniform1f(locColor1 , 0.7);
            glUniform1f(locColor2 , 0 );
            glUniform1f(locColor3 , 0.7);
        }
        else{
            glUniform1f(locColor1 , 0 );
            glUniform1f(locColor2 , 0.7);
            glUniform1f(locColor3 , 0);
        }

        glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT , 0 );
    }
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
    vertices[ 0 ] = QVector4D(  1,  1 ,1,1);
    vertices[ 1 ] = QVector4D( -1,  1 ,1,1);
    vertices[ 2 ] = QVector4D( -1, -1 ,1,1);
    vertices[ 3 ] = QVector4D(  1, -1 ,1,1);

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

void OpenGLWigget::keyPressEvent(QKeyEvent *event){
    switch (event->key() ) {
        case Qt::Key_W:
            playerPosYOffset =  2.0f;
            return;
        case Qt::Key_S:
            playerPosYOffset = -2.0f;
            return;
        case Qt::Key_Space:{
            this->create_projectiles(-0.75f, this->playerPosY , 0.2f , 0 , 0.1 , this->projectilesBaseType , 1 );
            return;
        }
        case Qt::Key_Q:{
            this->projectilesBaseType = this->projectilesBaseType+ 1;
            if(this->projectilesBaseType > 2 ) this->projectilesBaseType = 0;
            emit updateGun(QString("Gun: %1").arg(int(this->projectilesBaseType)));
            return;
        }
        case Qt::Key_Escape:
            QApplication::quit();
            return;
    }
}

void OpenGLWigget::keyReleaseEvent( QKeyEvent *event ){
    switch (event->key() ) {
        case Qt::Key_W:
        case Qt::Key_S:
            playerPosYOffset = 0;
            return;
        case Qt::Key_Escape:
            QApplication::quit();
        return;
    }
}

void OpenGLWigget::animate(){
    float elTime{this->elapsedTimer.restart()/1000.0f};

    this->target[_Y_POS_] += this->target[_Y_OFF_SET_]*elTime;
    if( this->target[_Y_POS_]>= .8f){
        this->target[_Y_POS_] =.8f;
        this->target[_Y_OFF_SET_] *= -1;
    }else if( this->target[_Y_POS_] <= -.8f){
        this->target[_Y_POS_] =-.8f;
        this->target[_Y_OFF_SET_] *= -1;
    }

    this->playerPosY += this->playerPosYOffset*elTime;
    if( this->playerPosY >= .8f){ this->playerPosY =.8f;}
    else if( this->playerPosY <= -.8f){this->playerPosY =-.8f;}

    // projectilesPos
    //      movimento
    for( auto i=0; i < this->projectiles.size(); i++ ){
        if( this->projectiles[i][_TYPE_] <= TYPE_PROJECTILES_BASE + 0.1 ){}
        else if( this->projectiles[i][_TYPE_] <= TYPE_PROJECTILES_DUAL + 0.1 ){
            // caso passar o meio ele vai dividir o tamanho e continuar como dois
            this->projectiles[i][_SIZE_] = -this->projectiles[i][_X_POS_]/15 + 0.05;
            if( this->projectiles[i][_X_POS_] > 0 ){
                this->create_projectiles(   this->projectiles[i][_X_POS_],        this->projectiles[i][_Y_POS_] + 0.01,
                                            this->projectiles[i][_X_OFF_SET_]*2,  this->projectiles[i][_Y_OFF_SET_]+0.03,
                                            this->projectiles[i][_SIZE_]*0.5,     TYPE_PROJECTILES_BASE , 1);
                this->create_projectiles(   this->projectiles[i][_X_POS_],        this->projectiles[i][_Y_POS_] -0.01,
                                            this->projectiles[i][_X_OFF_SET_]*2,  this->projectiles[i][_Y_OFF_SET_]-0.03,
                                            this->projectiles[i][_SIZE_]*0.5,     TYPE_PROJECTILES_BASE , 1);
                remove_projectiles( i );
                i = this->projectiles.size();
            }
        }else if( this->projectiles[i][_TYPE_] <= TYPE_PROJECTILES_LATERAL + 0.1 ){
            if( this->projectiles[i][_X_POS_] > -0.2f ){
                this->create_projectiles(   this->projectiles[i][_X_POS_],        this->projectiles[i][_Y_POS_] + 0.01,
                                            this->projectiles[i][_X_OFF_SET_]*2,  this->projectiles[i][_Y_OFF_SET_]+0.15 ,
                                            this->projectiles[i][_SIZE_]*0.5,     TYPE_PROJECTILES_LATERAL_2 , 1);
                this->create_projectiles(   this->projectiles[i][_X_POS_],        this->projectiles[i][_Y_POS_] -0.01,
                                            this->projectiles[i][_X_OFF_SET_]*2,  this->projectiles[i][_Y_OFF_SET_]-0.15,
                                            this->projectiles[i][_SIZE_]*0.5,     TYPE_PROJECTILES_LATERAL_2 , 1);
                remove_projectiles( i );
                i = this->projectiles.size();
            }
        }else if( this->projectiles[i][_TYPE_] <= TYPE_PROJECTILES_LATERAL_2 + 0.1 ){
            if( this->projectiles[i][_X_POS_] > 0.37f ){
                this->projectiles[i][_Y_OFF_SET_] = this->projectiles[i][_Y_OFF_SET_]*-1 +(target[_Y_POS_] - this->projectiles[i][_Y_POS_])/2;
                this->projectiles[i][_TYPE_] = TYPE_PROJECTILES_DUAL;
            }
        }



        this->projectiles[i][_X_POS_] += this->projectiles[i][_X_OFF_SET_]*elTime;
        this->projectiles[i][_Y_POS_] += this->projectiles[i][_Y_OFF_SET_]*elTime;
    }
    //      Hit no fim
    for( unsigned long i=0; i < this->projectiles.size(); i++ ){
        if( this->projectiles[i][_X_POS_] > this->target[_X_POS_] - this->target[_SIZE_] ){
            if( this->target[_Y_POS_] - this->target[_SIZE_] < this->projectiles[i][_Y_POS_]  && this->projectiles[i][_Y_POS_]  < this->target[_Y_POS_] + this->target[_SIZE_] ){
                this->shootingHits++;
                this->level++;
                if(target[_Y_OFF_SET_] > 0 ){ target[_Y_OFF_SET_] += float(level)/10.f;}
                else{target[_Y_OFF_SET_] -= float(level)/10.f;}
                target[_SIZE_] *= 0.95;
                if(target[_SIZE_] < 0.05){ target[_SIZE_] = 0.05f; }

                target[_COLOR_] = target[_COLOR_]*0.85;
                emit updateHitsLabel(QString("Hits %1").arg(this->shootingHits));
            }else{
                target[_COLOR_] = target[_COLOR_] + 0.1;
                if( target[_COLOR_] > 1 ){ target[_COLOR_] = 1.0f;  }

                this->level--;
                if( this->level <0 ){ level = 0; target[_Y_OFF_SET_] = 0.2f; target[_SIZE_] = 0.2f; }
                else{target[_SIZE_] *= 1.05;}
            }
            emit updateLevel(QString("Lv: %1").arg(this->level));

            this->remove_projectiles( i );
            i--;
        }
    }
    update();
}

void OpenGLWigget::create_projectiles(float var1,float var2,float var3 , float var4 ,float var5,float var6,float var7 ){
    std::array<float,7> *e = new std::array<float,7>();
    if( var6 == TYPE_PROJECTILES_BASE ){
        *e = {var1, var2 , var3*2 , var4 , var5 , var6 , var7 };
    }else{
        *e = {var1, var2 , var3 , var4 , var5 , var6 , var7 };
    }
    this->projectiles.push_back( *e );
}

void OpenGLWigget::remove_projectiles( unsigned long i ){
    auto it = projectiles.begin() + i;
    this->projectiles.erase(it);
}
