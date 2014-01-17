include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SNDFILESOURCE_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../alsaframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -lalsaframes_debug
else:unix:LIBS += -L../../../bin \
    -lalsaframes

LIBS += -lsndfile

SOURCES += \
    src/sndfilesource.cpp

HEADERS += \
    include/sndfilesource.h

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
