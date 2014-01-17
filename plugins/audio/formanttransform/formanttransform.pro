include(../../../config.pri)
QT += multimedia
QT -= gui

TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += FORMANTTRANSFORM_LIBRARY

SOURCES += \
    src/formanttransform.cpp

HEADERS += \
    include/formanttransform.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../audioframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -laudioframes_debug
else:unix:LIBS += -L../../../bin \
    -laudioframes

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
