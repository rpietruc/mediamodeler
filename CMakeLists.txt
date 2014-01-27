#cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 2.8)
project(QMM)

message(STATUS "System: " ${CMAKE_HOST_SYSTEM_NAME} " " ${CMAKE_HOST_SYSTEM_VERSION})
message(STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

set(QMM_INCLUDE_PATH "include/qmm")
set(QMM_PLUGINS_PATH "share/qmm/plugins")
set(QMM_INI_PATH "share/qmm/ini")

add_definitions(-DINSTALL_PLUGINS="${CMAKE_INSTALL_PREFIX}/${QMM_PLUGINS_PATH}")
add_definitions(-DINSTALL_INI="${CMAKE_INSTALL_PREFIX}/${QMM_INI_PATH}")

set(CMAKE_VERBOSE_MAKEFILE true)
add_definitions(-Wall)

include_directories(${PROJECT_SOURCE_DIR}/lib)

subdirs(lib)
subdirs(app)
subdirs(alsa)
subdirs(av)
subdirs(audio)
subdirs(itk)
subdirs(opencv)
subdirs(ini)
