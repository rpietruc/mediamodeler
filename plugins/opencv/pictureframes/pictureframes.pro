include(../../config.pri)
TEMPLATE = lib
CONFIG += staticlib
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)

SOURCES += \
    src/pictureframes.cpp

HEADERS += \
    include/pictureframes.h

DEPENDPATH = include
DEPENDPATH += ../../../lib/src
INCLUDEPATH = $$DEPENDPATH

CONFIG += link_pkgconfig
PKGCONFIG = opencv

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_LIBS}
INSTALLS = target

headers.files  = $${HEADERS}
headers.path   = $${QMM_INSTALL_HEADERS}
INSTALLS += headers
