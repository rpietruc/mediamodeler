#cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME qmediamodeler)
project(${PROJECT_NAME})

#message(STATUS "System: " ${CMAKE_HOST_SYSTEM_NAME} " " ${CMAKE_HOST_SYSTEM_VERSION})
#message(STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

#set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_VERBOSE_MAKEFILE true)

set(PLUGINS_PATH "share/qmediamodeler/plugins")
set(INI_PATH "share/qmediamodeler/ini")

find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES}
    ${PROJECT_SOURCE_DIR}/../core/include)
add_definitions(${Qt5Widgets_DEFINITIONS}
    -DINSTALL_PLUGINS="${CMAKE_INSTALL_PREFIX}/${PLUGINS_PATH}"
    -DINSTALL_INI="${CMAKE_INSTALL_PREFIX}/${INI_PATH}"
    -Wall)
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(${PROJECT_NAME}_SRC
    src/main.cpp
    src/mainwindow.cpp
    src/fileguielement.cpp
    src/guielementlist.cpp
    src/propertiesbox.cpp)
set(${PROJECT_NAME}_INCLUDE
    src/mainwindow.h
    src/fileguielement.h
    src/guielementbase.h
    src/guielementlist.h
    src/propertiesbox.h)
set(${PROJECT_NAME}_FORMS
    forms/mainwindow.ui
    forms/fileguielement.ui
    forms/guielementlist.ui)
set(${PROJECT_NAME}_INI
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

qt5_wrap_ui(SOURCES_FORMS_H ${${PROJECT_NAME}_FORMS})
qt5_wrap_cpp(SOURCES_MOC ${${PROJECT_NAME}_INCLUDE})
add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SRC}
    ${${PROJECT_NAME}_INI}
    ${${PROJECT_NAME}_INCLUDE}
    ${SOURCES_FORMS_H}
    ${SOURCES_MOC})

link_directories(${PROJECT_SOURCE_DIR}/../core)
find_library(MODELCORE_LIB modelcore)
target_link_libraries(${PROJECT_NAME}
    ${Qt5Widgets_LIBRARIES}
    ${MODELCORE_LIB})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${${PROJECT_NAME}_INI} DESTINATION ${INI_PATH})
