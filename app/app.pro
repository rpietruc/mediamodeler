include(../config.pri)
QT += widgets
TARGET = mediamodeler
CONFIG += console
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
CONFIG -= app_bundle
TEMPLATE = app

DEPENDPATH = include
DEPENDPATH += ../core/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../bin \
    -lmodelcore_debug
else:unix:LIBS += -L../bin \
    -lmodelcore

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/fileguielement.cpp \
    src/guielementlist.cpp

HEADERS += \
    include/mainwindow.h \
    include/fileguielement.h \
    include/guielementbase.h \
    include/guielementlist.h

FORMS += \
    forms/mainwindow.ui \
    forms/fileguielement.ui \
    forms/guielementlist.ui

DESTDIR = ../bin

DEFINES += INSTALL_PLUGINS=\\\"$${QMM_INSTALL_PLUGINS}\\\"
target.path = $${QMM_INSTALL_BIN}
INSTALLS = target
