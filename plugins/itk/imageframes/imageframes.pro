include(../../../config.pri)
TEMPLATE = lib
CONFIG += staticlib
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)

SOURCES += \
    src/imageframes.cpp

HEADERS += \
    include/imageframes.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
INCLUDEPATH += $$DEPENDPATH
#INCLUDEPATH += /usr/include/ITK-4.3

LIBS += -lITKCommon-4.3 \
    -litkvnl-4.3

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_LIBS}
INSTALLS = target

headers.files  = $${HEADERS}
headers.path   = $${QMM_INSTALL_HEADERS}
INSTALLS += headers
