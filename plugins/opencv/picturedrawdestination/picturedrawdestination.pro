include(../../../config.pri)
QT += widgets
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += PICTUDRAWDESTINATION_LIBRARY

SOURCES += \
    src/picturedrawdestination.cpp \
    src/drawwidget.cpp

HEADERS += \
    include/picturedrawdestination.h \
    src/drawwidget.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../pictureframes/include
INCLUDEPATH = $$DEPENDPATH

FORMS += \
    forms/drawwidget.ui

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
