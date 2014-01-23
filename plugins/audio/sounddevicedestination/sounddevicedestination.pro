include(../../../config.pri)
QT += multimedia
TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SOUNDDEVICEDESTINATION_LIBRARY

SOURCES += \
    src/sounddevicedestination.cpp \
    src/audiooutputdevice.cpp

HEADERS += \
    include/sounddevicedestination.h \
    include/audiooutputdevice.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../audioframes/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../audioframes \
#    -laudioframes_debug
#else:unix:
LIBS += -L../audioframes \
    -laudioframes

LIBS += -lasound

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
