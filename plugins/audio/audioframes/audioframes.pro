include(../../../config.pri)
QT += multimedia
TEMPLATE = lib
CONFIG += staticlib
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)

DEPENDPATH = include
DEPENDPATH += ../../../core/include
INCLUDEPATH = $$DEPENDPATH

SOURCES = \
    src/formantframe.cpp \
    src/spectrumframe.cpp \
    src/filterframe.cpp \
    src/soundframe.cpp
HEADERS = \
    include/formantframe.h \
    include/spectrumframe.h \
    include/filterframe.h \
    include/soundframe.h

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_LIBS}
INSTALLS = target

headers.files  = $${HEADERS}
headers.path   = $${QMM_INSTALL_HEADERS}
INSTALLS += headers
