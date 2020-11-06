#ifndef OPENGLWIGGET_H
#define OPENGLWIGGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTimer>
#include <QVector4D>
#include <QFile>
#include <QDebug>
#include <math.h>

class OpenGLWigget : public QOpenGLWidget , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWigget(QWidget *parent = nullptr);
    ~OpenGLWigget();
    void initializeGL() override;
    void resizeGL( int w , int h ) override;
    void paintGL();

signals:

public slots:
    void modificarCorDoFundo(bool);

private:
    QTimer timer;
    bool blinkFlag{false};

    GLuint vboVertices1{0};
    GLuint vboColors1{0};
    GLuint eboIndices1{0};
    GLuint vao1{0};

    GLuint vboVertices2{0};
    GLuint vboColors2{0};
    GLuint eboIndices2{0};
    GLuint vao2{0};

    GLuint vboVertices3{0};
    GLuint vboColors3{0};
    GLuint eboIndices3{0};
    GLuint vao3{0};


    std::vector<QVector4D> vertices1;
    std::vector<QVector4D> colors1;
    std::vector<GLuint> indices1;

    std::vector<QVector4D> vertices2;
    std::vector<QVector4D> colors2;
    std::vector<GLuint> indices2;

    std::vector<QVector4D> vertices3;
    std::vector<QVector4D> colors3;
    std::vector<GLuint> indices3;

    GLuint  sharderProgram{0};

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    bool resultadoCompilacaoShader( GLuint );
    bool resultadoCompilacaoPrograma( GLuint );

    GLuint criandoShader( char * , GLenum );

};

#endif // OPENGLWIGGET_H
