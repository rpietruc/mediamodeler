include(../config.pri)
TARGET = modelcore
TEMPLATE = lib
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += MEDIAMODEL_LIBRARY

DEPENDPATH = include
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    src/matrixframe.cpp \
    src/modelcreator.cpp \
    src/elementbase.cpp

HEADERS += \
    include/matrixframe.h \
    include/modelcreator.h \
    include/elementbase.h \
    include/framebase.h

DESTDIR = ../bin

target.path = $${QMM_INSTALL_LIBS}
INSTALLS += target

headers.files  = $${HEADERS}
headers.path   = $${QMM_INSTALL_HEADERS}
INSTALLS += headers
