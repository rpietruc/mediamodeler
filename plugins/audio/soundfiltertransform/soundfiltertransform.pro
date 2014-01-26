include(../../config.pri)
QT += multimedia
TEMPLATE = lib
CONFIG += plugin
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SOUNDFILTERTRANSFORM_LIBRARY

SOURCES += \
    src/soundfiltertransform.cpp

HEADERS +=\
    include/soundfiltertransform.h

DEPENDPATH = include
DEPENDPATH += ../audioframes/include
DEPENDPATH += ../../../lib/src
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../audioframes \
#    -laudioframes_debug
#else:unix:
LIBS += -L../audioframes \
    -laudioframes

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
