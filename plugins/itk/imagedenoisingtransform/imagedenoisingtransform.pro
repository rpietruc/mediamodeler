include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += IMAGEDENOISINGTRANSFORM_LIBRARY

SOURCES += \
    src/imagedenoisingtransform.cpp

HEADERS += \
    include/imagedenoisingtransform.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../imageframes/include
INCLUDEPATH += $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -limageframes_debug
else:unix:LIBS += -L../../../bin \
    -limageframes

LIBS += -lITKCommon-4.3 \
    -litkvnl-4.3

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
