#ifndef OPENGLWIGGET_H
#define OPENGLWIGGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTimer>
#include <QElapsedTimer>
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
    void animate();

private:
    QTimer timer;
    QElapsedTimer elapsedTimer;

    GLuint vboVertices{0};
    GLuint vboColors{0};
    GLuint eboIndices{0};
    GLuint vao{0};

    std::vector<QVector4D> vertices;
    std::vector<QVector4D> colors;
    std::vector<GLuint> indices;

    GLuint  sharderProgram{0};

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    bool resultadoCompilacaoShader( GLuint );
    bool resultadoCompilacaoPrograma( GLuint );

    GLuint criandoShader( char * , GLenum );


    float playerPosYOffset{.2};
    float playerPosY{0};
    float targetPosYOffset{.2};
    float targetPosY{0};
};

#endif // OPENGLWIGGET_H
