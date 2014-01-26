project(QMM_ALSA)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_library(asound REQUIRED)
find_library(sndfile REQUIRED)
include_directories(${Qt5Core_INCLUDES})
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

set(QMM_ALSA_SRC
    alsaframe.cpp
    alsasource.cpp
    alsadestination.cpp
    alsadummysource.cpp
    sndfilesource.cpp
    sndfiledestination.cpp)

set(QMM_ALSA_INC
    alsaframe.h
    alsasource.h
    alsadestination.h
    alsadummysource.h
    sndfilesource.h
    sndfiledestination.h)

add_library(alsaframes STATIC alsaframe.cpp alsaframe.h)
target_link_libraries(alsaframes ${Qt5Core_LIBRARIES})
install(TARGETS alsaframes ARCHIVE DESTINATION lib)

add_library(alsasource SHARED alsasource.cpp alsasource.h)
target_link_libraries(alsasource modelcore alsaframes ${Qt5Core_LIBRARIES} asound)
install(TARGETS alsasource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(alsadestination SHARED alsadestination.cpp alsadestination.h)
target_link_libraries(alsadestination modelcore alsaframes ${Qt5Core_LIBRARIES} asound)
install(TARGETS alsadestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(alsadummysource SHARED alsadummysource.cpp alsadummysource.h)
target_link_libraries(alsadummysource modelcore alsaframes ${Qt5Core_LIBRARIES})
install(TARGETS alsadummysource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(sndfilesource SHARED sndfilesource.cpp sndfilesource.h)
target_link_libraries(sndfilesource modelcore alsaframes ${Qt5Core_LIBRARIES} sndfile)
install(TARGETS sndfilesource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(sndfiledestination SHARED sndfiledestination.cpp sndfiledestination.h)
target_link_libraries(sndfiledestination modelcore alsaframes ${Qt5Core_LIBRARIES} sndfile)
install(TARGETS sndfiledestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

