#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->figura = new Figura();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TrocaForma(int tipo ){
    this->figura->set_get_tipo(tipo);
    update(); // chama o paintEvent
}

void MainWindow::TrocaCorBlue(int cor ){
    if( 0 <= cor && cor < 256 ){ this->figura->set_color_blue(cor); }
    update();
}
void MainWindow::TrocaCorRed(int cor ){
    if( 0 <= cor && cor < 256 ){ this->figura->set_color_red(cor); }
    update();
}
void MainWindow::TrocaCorGreen(int cor ){
    if( 0 <= cor && cor < 256 ){ this->figura->set_color_green(cor); }
    update();
}
void MainWindow::TrocaSaturacao(int cor ){
    if( 0 <= cor && cor < 256 ){ this->figura->set_color_saturation(cor); }
    update();
}
void MainWindow::TrocaX(int cor ){
    if( 10 <= cor && cor < 90 ){ this->figura->set_x(cor); }
    update();
}
void MainWindow::TrocaY(int cor ){
    if( 10 <= cor && cor < 90 ){ this->figura->set_y(cor); }
    update();
}
void MainWindow::TrocaTamanho1(int cor ){
    if( 10 <= cor && cor < 40 ){ this->figura->set_size_1(cor); }
    update();
}
void MainWindow::TrocaTamanho2(int cor ){
    if( 10 <= cor && cor < 40 ){ this->figura->set_size_2(cor); }
    update();
}

void MainWindow::paintEvent(QPaintEvent * ){
    QPainter painter(this);
    painter.setBrush(QColor( figura->get_color_red() , figura->get_color_green() , figura->get_color_blue() , this->figura->get_color_saturation()));

    if( this->figura->get_tipo() == CIRCULO ){
        painter.drawEllipse(figura->get_x(),figura->get_y(),figura->get_size_1(),figura->get_size_2());
    }
    else if( this->figura->get_tipo() == QUADRADO ){
        painter.drawRect(figura->get_x(),figura->get_y(),figura->get_size_1(),figura->get_size_2());
    }
     else if( this->figura->get_tipo() == TRIANGULO ){
        QPoint points_3[3] = {
            QPoint(10 + figura->get_x() + figura->get_size_1() , 10 + figura->get_y()),
            QPoint(20 + figura->get_x() + figura->get_size_1() , 20 + figura->get_y() + figura->get_size_2()),
            QPoint(10 + figura->get_x()                        , 20 + figura->get_y() + figura->get_size_2())
        };
        painter.drawPolygon( points_3 , 3 );
    }else if( this->figura->get_tipo() == PERSPECTIVA ){
        QPoint points_5[5] = {
            QPoint(10 + figura->get_x()                        , 10 + figura->get_y()                       ),
            QPoint(10 + figura->get_x() + figura->get_size_1() , 10 + figura->get_y() ),
                        QPoint(100                                          , 60),
            QPoint(10 + figura->get_x() + figura->get_size_1() , 20 + figura->get_y() + figura->get_size_2()),
            QPoint(10 + figura->get_x()                        , 20 + figura->get_y() + figura->get_size_2())
        };
        painter.drawPolygon( points_5 , 5 );
        QPoint points_2[2] = {
            QPoint(10 + figura->get_x() + figura->get_size_1() , 10 + figura->get_y() ),
            QPoint(10 + figura->get_x() + figura->get_size_1() , 20 + figura->get_y() + figura->get_size_2())
        };
        painter.drawPolyline(points_2, 2);
    }else if( this->figura->get_tipo() == LINHA ){
        QPoint points_2[2] = {
            QPoint(10 + figura->get_x() + figura->get_size_1() , 10 + figura->get_y() ),
            QPoint(10 + figura->get_x()                        , 20 + figura->get_y() + figura->get_size_2())
        };
        painter.drawPolyline(points_2, 2);
    }


}
