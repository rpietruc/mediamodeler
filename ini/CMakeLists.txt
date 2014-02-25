cmake_minimum_required(VERSION 2.8)
project(QMM_INI)

set(QMM_APP_INI
#    linespectralpairsplot.ini.old
#    linespectralpairswrite.ini.old
#    mouthcontourdetection.ini.old
#    mouthdetection.ini.old
#    mouthpointsdetection.ini.old
#    pointssave.ini.old
#    pointsview.ini.old
#    wavmonocopy.ini.old
    alsaloop.ini
    areasplot.ini
    audioloop.ini
    audioplayer.ini
    audioplot.ini
    audiorecord.ini
    crosssectionalareas.ini
    dummyplayer.ini
    facedetection.ini
    formants.ini
    grabframes.ini
    imageadd.ini
    imageand.ini
    imageenhancement.ini
    imagegeneration.ini
    imageskeletonization.ini
    lpc.ini
    movieplayer.ini
    pictureclustering.ini
    picturecontour.ini
    pictureedgedetection.ini
    picturegradient.ini
    picturesegmentation.ini
    picturesmoothing.ini
    pictureview.ini
    render.ini
    spectrum.ini
    videoplayer.ini
    picturewatershed.ini
    imagewatershed.ini
    imagecontour.ini
    wavcopy.ini
    wavfilter.ini
    wavplayer.ini)

install(FILES ${QMM_APP_INI} DESTINATION ${QMM_INI_PATH})
