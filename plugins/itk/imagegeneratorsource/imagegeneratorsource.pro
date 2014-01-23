include(../../../config.pri)
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += IMAGEGENERATORSOURCE_LIBRARY

SOURCES += \
    src/imagegeneratorsource.cpp

HEADERS += \
    include/imagegeneratorsource.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../imageframes/include
INCLUDEPATH += $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../../../core \
#    -limageframes_debug
#else:unix:
LIBS += -L../imageframes \
    -limageframes

LIBS += -lITKCommon-4.3 \
    -litkvnl-4.3

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
