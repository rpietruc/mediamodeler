TEMPLATE = lib
CONFIG += staticlib
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)

SOURCES += \
    src/linecontour.cpp \
    src/ellipsecontour.cpp \
    src/segmentcontour.cpp

HEADERS += \
    include/contourpoints.h \
    include/linecontour.h \
    include/ellipsecontour.h \
    include/contourapprox.h \
    include/segmentcontour.h \
    include/ofVec2f.h

DEPENDPATH = include
INCLUDEPATH += $$DEPENDPATH

CONFIG += link_pkgconfig
PKGCONFIG = opencv

DESTDIR = ../../../bin

target.path = $${QMM_INSTALL_PLUGINS}
INSTALLS = target
