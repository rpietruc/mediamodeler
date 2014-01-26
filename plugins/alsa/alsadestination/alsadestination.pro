include(../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += ALSADESTINATION_LIBRARY

SOURCES += \
    src/alsadestination.cpp

HEADERS += \
    include/alsadestination.h

DEPENDPATH = include
DEPENDPATH += ../../../lib/src
DEPENDPATH += ../alsaframes/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../alsaframes \
#    -lalsaframes_debug
#else:unix:
LIBS += -L../alsaframes \
    -lalsaframes

LIBS += -lasound

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
