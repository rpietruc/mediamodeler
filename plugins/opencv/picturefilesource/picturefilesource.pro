include(../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += PICTUREFILESOURCE_LIBRARY

SOURCES += \
    src/picturefilesource.cpp

HEADERS += \
    include/picturefilesource.h

DEPENDPATH = include
DEPENDPATH += ../../../lib/src
DEPENDPATH += ../pictureframes/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../../../core \
#    -lpictureframes_debug
#else:unix:
LIBS += -L../pictureframes \
    -lpictureframes

CONFIG += link_pkgconfig
PKGCONFIG = opencv

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
