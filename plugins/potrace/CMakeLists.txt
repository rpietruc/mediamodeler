project(QMM_POTRACE)

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

set(QMM_POTRACE_SRC
    potracegeneratorsource.cpp
    potracetracetransform.cpp
    potracefiledestination.cpp)

set(QMM_POTRACE_INC
    potracegeneratorsource.h
    potracetracetransform.h
    potracefiledestination.h)

add_library(potraceframes STATIC potraceframes.cpp potraceframes.h)
target_link_libraries(potraceframes ${Qt5Core_LIBRARIES} potrace)
install(TARGETS potraceframes ARCHIVE DESTINATION lib)
install(FILES potraceframes.h DESTINATION ${QMM_INCLUDE_PATH})

add_library(potracegeneratorsource SHARED potracegeneratorsource.cpp potracegeneratorsource.h)
target_link_libraries(potracegeneratorsource modelcore potraceframes ${Qt5Core_LIBRARIES} potrace)
install(TARGETS potracegeneratorsource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(potracetracetransform SHARED potracetracetransform.cpp potracetracetransform.h)
target_link_libraries(potracetracetransform modelcore potraceframes ${Qt5Core_LIBRARIES} potrace)
install(TARGETS potracetracetransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(potracefiledestination SHARED potracefiledestination.cpp potracefiledestination.h)
target_link_libraries(potracefiledestination modelcore potraceframes ${Qt5Core_LIBRARIES} potrace)
install(TARGETS potracefiledestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})
