QT -= gui
DEFINES += PLUGINTARGETNAME=$$TARGET
TARGET = soundextractchanneltransform
TEMPLATE = lib
CONFIG += plugin

DEFINES += SOUNDEXTRACTCHANNELTRANSFORM_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../modelcore/include
DEPENDPATH += ../../soundframes/include
INCLUDEPATH = $$DEPENDPATH

LIBS += -L../../../../bin \
    -lsoundframes

SOURCES += \
    src/soundextractchanneltransform.cpp

HEADERS +=\
    include/soundextractchanneltransform.h

DESTDIR = ../../../../bin
