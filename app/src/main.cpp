#include <QApplication>
#include <QTime>
#include "mainwindow.h"

int main(int argc, char *argv[])
    {
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QApplication::setOrganizationName("mediamodeler");
    QApplication::setApplicationName("app");
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
    }
