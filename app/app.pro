include(../config.pri)
QT += widgets
TARGET = qmediamodeler
CONFIG += console
CONFIG += debug_and_release
#CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_debug)
CONFIG -= app_bundle
TEMPLATE = app

DEPENDPATH = include
DEPENDPATH += ../core/include
INCLUDEPATH = $$DEPENDPATH

#CONFIG(debug, debug|release):unix:LIBS += -L../core \
#    -lmodelcore_debug
#else:unix:
LIBS += -L../core \
    -lmodelcore

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/fileguielement.cpp \
    src/guielementlist.cpp \
    src/elementbox.cpp

HEADERS += \
    include/mainwindow.h \
    include/fileguielement.h \
    include/guielementbase.h \
    include/guielementlist.h \
    include/elementbox.h

FORMS += \
    forms/mainwindow.ui \
    forms/fileguielement.ui \
    forms/guielementlist.ui

#DESTDIR = ../bin

#TODO: move project to CMake to get rid of this strange code
DEFINES += INSTALL_PLUGINS=\\\"$${QMM_INSTALL_PLUGINS}\\\"
DEFINES += INSTALL_INI=\\\"$${QMM_INSTALL_INI}\\\"
target.path = $${QMM_INSTALL_BIN}
INSTALLS = target

OTHER_FILES += \
#    ini/linespectralpairsplot.ini.old \
#    ini/linespectralpairswrite.ini.old \
#    ini/mouthcontourdetection.ini.old \
#    ini/mouthdetection.ini.old \
#    ini/mouthpointsdetection.ini.old \
#    ini/pointssave.ini.old \
#    ini/pointsview.ini.old \
#    ini/wavmonocopy.ini.old \
    ini/alsaloop.ini \
    ini/areasplot.ini \
    ini/audioloop.ini \
    ini/audioplayer.ini \
    ini/audioplot.ini \
    ini/audiorecord.ini \
    ini/crosssectionalareas.ini \
    ini/dummyplayer.ini \
    ini/facedetection.ini \
    ini/formants.ini \
    ini/grabframes.ini \
    ini/imageadd.ini \
    ini/imageand.ini \
    ini/imageenhancement.ini \
    ini/imagegeneration.ini \
    ini/imageskeletonization.ini \
    ini/lpc.ini \
    ini/movieplayer.ini \
    ini/pictureclustering.ini \
    ini/picturecontour.ini \
    ini/pictureedgedetection.ini \
    ini/picturegradient.ini \
    ini/picturesegmentation.ini \
    ini/picturesmoothing.ini \
    ini/pictureview.ini \
    ini/render.ini \
    ini/spectrum.ini \
    ini/videoplayer.ini \
    ini/watershed.ini \
    ini/wavcopy.ini \
    ini/wavfilter.ini \
    ini/wavplayer.ini

ini.files = $${OTHER_FILES}
ini.path  = $${QMM_INSTALL_INI}
INSTALLS += ini
