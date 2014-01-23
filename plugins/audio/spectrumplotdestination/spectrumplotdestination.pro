include(../../../config.pri)
QT += multimedia
QT += widgets
QT += printsupport

TEMPLATE = lib
CONFIG += plugin
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += SOUNDPLOTDESTINATION_LIBRARY

SOURCES += \
    src/plotdialog.cpp \
    src/spectrumplotdestination.cpp

HEADERS += \
    include/plotdialog.h \
    include/spectrumplotdestination.h

FORMS += \
    forms/plotdialog.ui

DEPENDPATH = include
DEPENDPATH += ../audioframes/include
DEPENDPATH += ../../../core/include
INCLUDEPATH = $$DEPENDPATH

LIBS += -lqcustomplot

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
