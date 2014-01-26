project(QMM_AV)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(FFMPEG_LIBS avutil avformat avcodec swscale swresample)

find_package(Qt5Core REQUIRED)
find_library(${FFMPEG_LIBS} REQUIRED)
include_directories(${Qt5Core_INCLUDES})
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

set(QMM_AV_FRM_SRC
    src/audioframe.cpp
    src/videoframe.cpp)

set(QMM_AV_FRM_INC
    src/audioframe.h
    src/videoframe.h)

set(QMM_AV_SRC
    ${QMM_AV_FRM_SRC}
    src/moviefilesource.cpp)

set(QMM_AV_INC
    ${QMM_AV_FRM_INC}
    src/moviefilesource.h)

add_library(movieframes STATIC ${QMM_AV_FRM_SRC} ${QMM_AV_FRM_INC})
target_link_libraries(movieframes ${Qt5Core_LIBRARIES} ${FFMPEG_LIBS})
install(TARGETS movieframes ARCHIVE DESTINATION lib)

add_library(moviefilesource SHARED src/moviefilesource.cpp src/moviefilesource.h)
target_link_libraries(moviefilesource modelcore movieframes ${Qt5Core_LIBRARIES} ${FFMPEG_LIBS})
install(TARGETS moviefilesource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})
