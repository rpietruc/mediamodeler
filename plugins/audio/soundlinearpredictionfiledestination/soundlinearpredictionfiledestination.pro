QT += core \
    gui

TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SOUNDLINEARPREDICTIONFILEDESTINATION_LIBRARY

SOURCES += \
    src/soundlinearpredictionfiledestination.cpp

HEADERS += \
    include/soundlinearpredictionfiledestination.h

DEPENDPATH = include
DEPENDPATH += ../../../modelcore/include
DEPENDPATH += ../../soundframes/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../../../../bin \
#    -lsoundframes_debug
#else:unix:LIBS += -L../../../../bin \
#    -lsoundframes

DESTDIR = ../../../../bin
