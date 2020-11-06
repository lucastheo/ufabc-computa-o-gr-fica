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
#include <QKeyEvent>
#include <QApplication>

#define _X_POS_     0
#define _Y_POS_     1
#define _X_OFF_SET_ 2
#define _Y_OFF_SET_ 3
#define _SIZE_      4
#define _TYPE_      5
#define _COLOR_     6

#define TYPE_PROJECTILES_BASE 0
#define TYPE_PROJECTILES_DUAL 1
#define TYPE_PROJECTILES_LATERAL 2
#define TYPE_PROJECTILES_LATERAL_2 3


class OpenGLWigget : public QOpenGLWidget , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWigget(QWidget *parent = nullptr);
    ~OpenGLWigget();
    void initializeGL() override;
    void resizeGL( int w , int h ) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent( QKeyEvent *event ) override;

signals:
    void updateHitsLabel(QString);
    void updateGun(QString);
    void updateLevel(QString);
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
    float playerPosYOffset{0};
    float playerPosY{0};

    /* 0,1 ->pos
     * 2,3 ->offset
     * 5   ->size
     * 6   ->type
    */
    std::array<float,7> target ={0.8f,0,0,0.2f,0.2f,1,1};
    std::vector<std::array<float,7>> projectiles={};
    float projectilesBaseType = 0;
    int shootingHits{0};
    int level{0};

    void createVBOs();
    void createShaders();
    void destroyVBOs();
    void destroyShaders();
    bool resultadoCompilacaoShader( GLuint );
    bool resultadoCompilacaoPrograma( GLuint );
    GLuint criandoShader( char * , GLenum );
    void create_projectiles(float,float,float,float,float,float,float);
    void remove_projectiles( unsigned long );
};

#endif // OPENGLWIGGET_H
