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
add_definitions(-fPIC)
add_definitions(-std=c++11 -fext-numeric-literals)

include_directories(${PROJECT_SOURCE_DIR}/lib/core)
include_directories(${PROJECT_SOURCE_DIR}/lib/contourapproximation)
include_directories(${PROJECT_SOURCE_DIR}/lib/ellipsefitting)
include_directories(${PROJECT_SOURCE_DIR}/lib/pulsecoupledneuralnetwork)
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

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(plugins)
add_subdirectory(ini)
add_subdirectory(test)

# TESTING
enable_testing()
add_test("EllipseMergeTest" ${EXECUTABLE_OUTPUT_PATH}/ellipsemerge_test)
add_test("MultipleChannelPcnnTest" ${EXECUTABLE_OUTPUT_PATH}/multiplechannelpcnn_test)
add_test("RegionGrowingPcnnTest" ${EXECUTABLE_OUTPUT_PATH}/regiongrowingpcnn_test)
