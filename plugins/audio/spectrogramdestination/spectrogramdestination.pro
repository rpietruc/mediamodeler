include(../../../config.pri)
QT += multimedia
QT += widgets
QT += printsupport
TEMPLATE = lib
CONFIG += plugin
CONFIG += qwt
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SPECTROGRAMDESTINATION_LIBRARY

SOURCES += \
    src/spectrogramplot.cpp \
    src/spectrogramwindow.cpp \
    src/spectrogramdestination.cpp

HEADERS += \
    include/spectrogramplot.h \
    include/spectrogramwindow.h \
    include/spectrogramdestination.h

DEPENDPATH = include
DEPENDPATH += ../audioframes/include
DEPENDPATH += ../../../core/include
INCLUDEPATH = $$DEPENDPATH

CONFIG(debug, debug|release):unix:LIBS += -L../../../bin \
    -laudioframes_debug \
    -lmodelcore_debug
else:unix:LIBS += -L../../../bin \
    -laudioframes \
    -lmodelcore

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target