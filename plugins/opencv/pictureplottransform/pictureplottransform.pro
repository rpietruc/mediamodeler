QT -= gui
DEFINES += PLUGINTARGETNAME=$$TARGET
TARGET = pictureplottransform
TEMPLATE = lib
CONFIG += plugin

DEFINES += PICTUREPLOTTRANSFORM_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../modelcore/include
DEPENDPATH += ../../pictureframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG += link_pkgconfig
PKGCONFIG = opencv

LIBS += -L../../../../bin \
    -lpictureframes

SOURCES += \
    src/pictureplottransform.cpp

HEADERS += \
    include/pictureplottransform.h

DESTDIR = ../../../../bin
