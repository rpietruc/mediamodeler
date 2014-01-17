include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += MOVIEFILESOURCE_LIBRARY
DEFINES += __STDC_CONSTANT_MACROS

SOURCES += \
    src/moviefilesource.cpp

HEADERS += \
    include/moviefilesource.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../movieframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -lmovieframes_debug
else:unix:LIBS += -L../../../bin \
    -lmovieframes

LIBS += -lavutil \
    -lavformat \
    -lavcodec \
    -lswscale \
    -lswresample

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
