QT -= gui
DEFINES += PLUGINTARGETNAME=$$TARGET
TARGET = picturesnaketransform
TEMPLATE = lib
CONFIG += plugin

DEFINES += PICTURESNAKETRANSFORM_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../modelcore/include
DEPENDPATH += ../../pictureframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG += link_pkgconfig
PKGCONFIG = opencv

LIBS += -L../../../../bin \
    -lpictureframes

SOURCES += \
    src/picturesnaketransform.cpp

HEADERS += \
    include/picturesnaketransform.h

DESTDIR = ../../../../bin
