include(../../../config.pri)
QT += multimedia
QT += widgets
QT += printsupport

TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SOUNDPLOTDESTINATION_LIBRARY

SOURCES += \
    src/soundplotdestination.cpp \
    src/plotdialog.cpp

HEADERS += \
    include/soundplotdestination.h \
    include/plotdialog.h

FORMS += \
    forms/plotdialog.ui

DEPENDPATH = include
DEPENDPATH += ../audioframes/include
DEPENDPATH += ../../../core/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -lmodelcore_debug
else:unix:LIBS += -L../../../bin \
    -lmodelcore

LIBS += -lqcustomplot

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
