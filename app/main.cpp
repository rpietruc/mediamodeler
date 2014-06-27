#include <QApplication>
#include <QTime>
#include "mainwindow.h"
#include <stdio.h>
//#include <stdlib.h>

const char* msgTypeName(QtMsgType aMsgType)
    {
    static const char* msg[] = { "Debug", "Warning", "Critical", "Fatal" };
    return aMsgType < sizeof(msg)/sizeof(msg[0]) ? msg[aMsgType] : "Unknown";
    }

void messageOutput(QtMsgType aMsgType, const QMessageLogContext &aMessageLogContext, const QString &aMsg)
    {
    fprintf(stderr, "%s: %s (%s %s:%d)\n", msgTypeName(aMsgType), qPrintable(aMsg), aMessageLogContext.function, aMessageLogContext.file, aMessageLogContext.line);
    fflush(stderr);
    if (aMsgType == QtFatalMsg)
        abort();
    }

int main(int argc, char *argv[])
    {
    qInstallMessageHandler(messageOutput);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QApplication::setOrganizationName("qmediamodeler");
    QApplication::setApplicationName("app");
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
    }
