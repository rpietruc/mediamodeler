project(QMM_MLPACK)

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

find_package(LibXml2 2.6.0 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

add_library(matrixfilesource SHARED matrixfilesource.cpp matrixfilesource.h)
target_link_libraries(matrixfilesource modelcore armadilloframes ${Qt5Core_LIBRARIES} mlpack)
install(TARGETS matrixfilesource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(matrixfiledestination SHARED matrixfiledestination.cpp matrixfiledestination.h)
target_link_libraries(matrixfiledestination modelcore armadilloframes ${Qt5Core_LIBRARIES} mlpack)
install(TARGETS matrixfiledestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

