include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += ALSASOURCE_LIBRARY

SOURCES += \
    src/alsasource.cpp

HEADERS += \
    include/alsasource.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../alsaframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -lalsaframes_debug
else:unix:LIBS += -L../../../bin \
    -lalsaframes

LIBS += -lasound

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
