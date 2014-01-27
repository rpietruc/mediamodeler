project(QMM_AV)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
if(Qt5Core_FOUND)
  include_directories(${Qt5Core_INCLUDES})
  add_definitions(${Qt5Core_DEFINITIONS})
  set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
  add_definitions(-DQT_PLUGIN)
  add_definitions(-DQT_SHARED)
endif(Qt5Core_FOUND)

find_library(avutil avutil REQUIRED)
find_library(avformat avformat REQUIRED)
find_library(avcodec avcodec REQUIRED)
find_library(swscale swscale REQUIRED)
find_library(swresample swresample REQUIRED)
set(FFMPEG_LIBS ${avutil} ${avformat} ${avcodec} ${swscale} ${swresample})

set(MOVIEFRAMES_SRC
    audioframe.cpp
    videoframe.cpp)

set(MOVIEFRAMES_INC
    audioframe.h
    videoframe.h)

set(QMM_AV_SRC
    ${MOVIEFRAMES_SRC}
    moviefilesource.cpp)

set(QMM_AV_INC
    ${MOVIEFRAMES_INC}
    moviefilesource.h)

add_library(movieframes STATIC ${MOVIEFRAMES_SRC} ${MOVIEFRAMES_INC})
target_link_libraries(movieframes ${Qt5Core_LIBRARIES} ${FFMPEG_LIBS})
install(TARGETS movieframes ARCHIVE DESTINATION lib)
install(FILES ${MOVIEFRAMES_INC} DESTINATION ${QMM_INCLUDE_PATH})

add_library(moviefilesource SHARED moviefilesource.cpp moviefilesource.h)
target_link_libraries(moviefilesource modelcore movieframes ${Qt5Core_LIBRARIES} ${FFMPEG_LIBS})
install(TARGETS moviefilesource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})
