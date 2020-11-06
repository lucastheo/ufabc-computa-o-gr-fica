/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "openglwigget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    OpenGLWigget *openGLWidget;
    QLabel *labelHits;
    QLabel *labelGun;
    QLabel *labelLv;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(871, 665);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        openGLWidget = new OpenGLWigget(centralwidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(10, 60, 851, 581));
        openGLWidget->setFocusPolicy(Qt::StrongFocus);
        labelHits = new QLabel(centralwidget);
        labelHits->setObjectName(QString::fromUtf8("labelHits"));
        labelHits->setGeometry(QRect(20, 10, 181, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        font.setPointSize(20);
        labelHits->setFont(font);
        labelGun = new QLabel(centralwidget);
        labelGun->setObjectName(QString::fromUtf8("labelGun"));
        labelGun->setGeometry(QRect(610, 10, 251, 41));
        labelGun->setFont(font);
        labelLv = new QLabel(centralwidget);
        labelLv->setObjectName(QString::fromUtf8("labelLv"));
        labelLv->setGeometry(QRect(200, 10, 111, 31));
        labelLv->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 871, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(openGLWidget, SIGNAL(updateHitsLabel(QString)), labelHits, SLOT(setText(QString)));
        QObject::connect(openGLWidget, SIGNAL(updateGun(QString)), labelGun, SLOT(setText(QString)));
        QObject::connect(openGLWidget, SIGNAL(updateLevel(QString)), labelLv, SLOT(setText(QString)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelHits->setText(QCoreApplication::translate("MainWindow", "Num Hits: 0", nullptr));
        labelGun->setText(QCoreApplication::translate("MainWindow", "Gun: 1 ( Press Q)", nullptr));
        labelLv->setText(QCoreApplication::translate("MainWindow", "Lv: 0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
