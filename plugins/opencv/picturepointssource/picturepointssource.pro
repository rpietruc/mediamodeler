QT += core \
    gui

TARGET = picturepointssource
TEMPLATE = lib
CONFIG += plugin
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += PICTUREPOINTSSOURCE_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../modelcore/include
DEPENDPATH += ../../pictureframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG += link_pkgconfig
PKGCONFIG = opencv

LIBS += -L../../../../bin \
    -lpictureframes

SOURCES += \
    src/picturepointssource.cpp

HEADERS += \
    include/picturepointssource.h

DESTDIR = ../../../../bin

