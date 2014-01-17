include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += PICTUREAPPROXIMATIONTRANSFORM_LIBRARY

SOURCES += \
    src/pictureapproximationtransform.cpp

HEADERS += \
    include/pictureapproximationtransform.h

DEPENDPATH += include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../contourapproximation/include
DEPENDPATH += ../pictureframes/include
INCLUDEPATH += $$DEPENDPATH

CONFIG += link_pkgconfig
PKGCONFIG = opencv

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -lpictureframes_debug \
    -lcontourapproximation_debug
else:unix:LIBS += -L../../../bin \
    -lpictureframes \
    -lcontourapproximation

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
