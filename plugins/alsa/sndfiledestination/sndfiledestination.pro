include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SNDFILEDESTINATION_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../alsaframes/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../alsaframes \
#    -lalsaframes_debug
#else:unix:
LIBS += -L../alsaframes \
    -lalsaframes

LIBS += -lsndfile

SOURCES += \
    src/sndfiledestination.cpp

HEADERS += \
    include/sndfiledestination.h

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
