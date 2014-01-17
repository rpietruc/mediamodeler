QT -= gui
DEFINES += PLUGINTARGETNAME=$$TARGET
TARGET = soundlinespectralpairstransform
TEMPLATE = lib
CONFIG += plugin

DEFINES += SOUNDLINESPECTRALPAIRSTRANSFORM_LIBRARY

DEPENDPATH = include
DEPENDPATH += ../../../modelcore/include
DEPENDPATH += ../../soundframes/include
INCLUDEPATH = $$DEPENDPATH
INCLUDEPATH += /usr/local/include/speechinversion
INCLUDEPATH += /usr/local/src/speex-1.2rc1/libspeex

#CONFIG += link_pkgconfig
#PKGCONFIG = speex

LIBS += -L../../../../bin \
    -lsoundframes \
    -lspeechinversion

SOURCES += \
    src/soundlinespectralpairstransform.cpp

HEADERS += \
    include/soundlinespectralpairstransform.h

DESTDIR = ../../../../bin

TEMPLATE = lib
