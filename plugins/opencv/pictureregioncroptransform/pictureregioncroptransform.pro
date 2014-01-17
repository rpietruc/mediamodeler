QT       -= gui

TARGET = pictureregioncroptransform
TEMPLATE = lib
CONFIG += plugin
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += PICTUREREGIONCROPTRANSFORM_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../modelcore/include
DEPENDPATH += ../../pictureframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG += link_pkgconfig
PKGCONFIG = opencv

LIBS += -L../../../../bin \
    -lpictureframes

SOURCES += \
    src/pictureregioncroptransform.cpp

HEADERS += \
    include/pictureregioncroptransform.h

DESTDIR = ../../../../bin
