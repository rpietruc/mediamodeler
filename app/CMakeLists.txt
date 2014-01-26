project(QMM_APP)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(QMM_APP_SRC
    src/main.cpp
    src/mainwindow.cpp
    src/fileguielement.cpp
    src/guielementlist.cpp
    src/propertiesbox.cpp)
set(QMM_APP_INC
    src/mainwindow.h
    src/fileguielement.h
    src/guielementbase.h
    src/guielementlist.h
    src/propertiesbox.h)
set(QMM_APP_FRM
    forms/mainwindow.ui
    forms/fileguielement.ui
    forms/guielementlist.ui)
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

qt5_wrap_ui(QMM_APP_FRM_H ${QMM_APP_FRM})
add_executable(mediamodeler ${QMM_APP_SRC} ${QMM_APP_INC} ${QMM_APP_INI} ${QMM_APP_FRM_MOC})

link_directories(${CMAKE_BINARY_DIR})
target_link_libraries(mediamodeler ${Qt5Widgets_LIBRARIES} modelcore)

install(TARGETS mediamodeler RUNTIME DESTINATION bin)
install(FILES ${QMM_APP_INI} DESTINATION ${QMM_INI_PATH})
