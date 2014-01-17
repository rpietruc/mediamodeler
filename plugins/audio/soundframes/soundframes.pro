QT -= gui
QT += multimedia

TEMPLATE = lib
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += SOUNDFRAMES_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../core/include
INCLUDEPATH = $$DEPENDPATH

SOURCES += \


DESTDIR = ../../../bin
