#ifndef OPENGLWIGGET_H
#define OPENGLWIGGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTimer>

class OpenGLWigget : public QOpenGLWidget , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWigget(QWidget *parent = nullptr);

    void initializeGL() override;
    void resizeGL( int w , int h ) override;
    void paintGL();
signals:

public slots:
    void modificarCorDoFundo(bool);

private:
    QTimer timer;
    bool blinkFlag{false};
};

#endif // OPENGLWIGGET_H
