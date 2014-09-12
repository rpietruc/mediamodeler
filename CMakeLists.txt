#cmake -DCMAKE_INSTALL_PREFIX=`pwd`/inst -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 2.8)
project(QMM)

message(STATUS "System: " ${CMAKE_HOST_SYSTEM_NAME} " " ${CMAKE_HOST_SYSTEM_VERSION})
message(STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

set(QMM_INCLUDE_PATH "include/qmm")
set(QMM_PLUGINS_PATH "share/qmm/plugins")
set(QMM_INI_PATH "share/qmm/ini")

add_definitions(-DINSTALL_PLUGINS="${CMAKE_INSTALL_PREFIX}/${QMM_PLUGINS_PATH}")
add_definitions(-DINSTALL_INI="${CMAKE_INSTALL_PREFIX}/${QMM_INI_PATH}")

#set(CMAKE_VERBOSE_MAKEFILE true)
add_definitions(-Wall)

include_directories(${PROJECT_SOURCE_DIR}/lib)
#set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS OFF)

# Output directories.
if(NOT LIBRARY_OUTPUT_PATH)
    set(LIBRARY_OUTPUT_PATH ${QMM_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
endif(NOT LIBRARY_OUTPUT_PATH)
if(NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${QMM_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
endif(NOT EXECUTABLE_OUTPUT_PATH)
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
message(STATUS "qmediamodeler: Example executables are stored in ${EXECUTABLE_OUTPUT_PATH}")

subdirs(lib)
subdirs(app)
subdirs(plugins)
subdirs(ini)
subdirs(test)
