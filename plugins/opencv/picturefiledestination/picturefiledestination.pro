include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += PICTUREFILEDESTINATION_LIBRARY

SOURCES += \
    src/picturefiledestination.cpp

HEADERS += \
    include/picturefiledestination.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../pictureframes/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -lpictureframes_debug
else:unix:LIBS += -L../../../bin \
    -lpictureframes

CONFIG += link_pkgconfig
PKGCONFIG = opencv

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target