include(../../config.pri)
TEMPLATE = lib
CONFIG += staticlib
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)

DEPENDPATH = include
DEPENDPATH += ../../../lib/src
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    src/alsaframe.cpp
HEADERS += \
    include/alsaframe.h

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_LIBS}
INSTALLS = target

headers.files  = $${HEADERS}
headers.path   = $${QMM_INSTALL_HEADERS}
INSTALLS += headers
