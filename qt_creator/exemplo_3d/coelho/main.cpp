#include "mainwindow.h"

#include <QApplication>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
#ifdef Q_OS_MAC
    formar.setVersion(4,1);
#else
    format.setVersion(4,0);
#endif

    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);

    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
