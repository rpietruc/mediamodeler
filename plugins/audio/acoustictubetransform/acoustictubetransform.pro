include(../../../config.pri)
QT += multimedia
TEMPLATE = lib
CONFIG += plugin
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
DEFINES += PLUGINTARGETNAME=$$TARGET
DEFINES += ACOUSTICTUBETRANSFORM_LIBRARY

SOURCES += \
    src/acoustictubetransform.cpp

HEADERS += \
    include/acoustictubetransform.h \
    include/filterconverter.h

DEPENDPATH = include
DEPENDPATH += ../../../core/include
DEPENDPATH += ../audioframes/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../audioframes \
#    -laudioframes_debug
#else:unix:
LIBS += -L../audioframes \
    -laudioframes

#DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
