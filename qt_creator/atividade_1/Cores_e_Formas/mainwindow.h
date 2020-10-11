#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDebug>
#include<QPainter>
#include <domain/figura.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

public slots:
    void TrocaForma(int);
    void TrocaCorBlue(int);
    void TrocaCorRed(int);
    void TrocaCorGreen(int);
    void TrocaSaturacao(int);
    void TrocaX(int);
    void TrocaY(int);
    void TrocaTamanho1(int);
    void TrocaTamanho2(int);

private:
    Figura *figura;
    Ui::MainWindow *ui;
    int codigoForma{1};
};
#endif // MAINWINDOW_H
