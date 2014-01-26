cmake_minimum_required(VERSION 2.8)
project(QMM_INI)

set(QMM_APP_INI
#    ini/linespectralpairsplot.ini.old
#    ini/linespectralpairswrite.ini.old
#    ini/mouthcontourdetection.ini.old
#    ini/mouthdetection.ini.old
#    ini/mouthpointsdetection.ini.old
#    ini/pointssave.ini.old
#    ini/pointsview.ini.old
#    ini/wavmonocopy.ini.old
    ini/alsaloop.ini
    ini/areasplot.ini
    ini/audioloop.ini
    ini/audioplayer.ini
    ini/audioplot.ini
    ini/audiorecord.ini
    ini/crosssectionalareas.ini
    ini/dummyplayer.ini
    ini/facedetection.ini
    ini/formants.ini
    ini/grabframes.ini
    ini/imageadd.ini
    ini/imageand.ini
    ini/imageenhancement.ini
    ini/imagegeneration.ini
    ini/imageskeletonization.ini
    ini/lpc.ini
    ini/movieplayer.ini
    ini/pictureclustering.ini
    ini/picturecontour.ini
    ini/pictureedgedetection.ini
    ini/picturegradient.ini
    ini/picturesegmentation.ini
    ini/picturesmoothing.ini
    ini/pictureview.ini
    ini/render.ini
    ini/spectrum.ini
    ini/videoplayer.ini
    ini/watershed.ini
    ini/wavcopy.ini
    ini/wavfilter.ini
    ini/wavplayer.ini)

install(FILES ${QMM_APP_INI} DESTINATION ${QMM_INI_PATH})

