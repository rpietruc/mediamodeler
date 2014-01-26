include(../../config.pri)
QT -= gui
QT += multimedia

TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SPECTRUMTRANSFORM_LIBRARY

SOURCES += \
    src/spectrumtransform.cpp

HEADERS += \
    include/spectrumtransform.h

DEPENDPATH = include
DEPENDPATH += ../../../lib/src
DEPENDPATH += ../audioframes/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../audioframes \
#    -laudioframes_debug
#else:unix:
LIBS += -L../audioframes \
    -laudioframes

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
