include(../../../config.pri)
TEMPLATE = lib
CONFIG += staticlib
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += __STDC_CONSTANT_MACROS

DEPENDPATH = include
DEPENDPATH += ../../../core/include
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    src/audioframe.cpp \
    src/videoframe.cpp
HEADERS += \
    include/audioframe.h \
    include/videoframe.h

LIBS += -lavutil \
    -lavformat \
    -lavcodec \
    -lswscale \
    -lswresample

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_LIBS}
INSTALLS = target

headers.files  = $${HEADERS}
headers.path   = $${QMM_INSTALL_HEADERS}
INSTALLS += headers
