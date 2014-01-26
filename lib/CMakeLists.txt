project(QMM_LIB)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDES})
add_definitions(${Qt5Core_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

set(QMM_LIB_SRC
    matrixframe.cpp
    modelcreator.cpp
    elementbase.cpp)

set(QMM_LIB_INC
    globaldefs.h
    matrixframe.h
    modelcreator.h
    elementbase.h
    framebase.h)

add_library(modelcore SHARED ${QMM_LIB_SRC} ${QMM_LIB_INC})
target_link_libraries(modelcore ${Qt5Core_LIBRARIES})

install(TARGETS modelcore LIBRARY DESTINATION lib)
install(FILES ${QMM_LIB_INC} DESTINATION ${QMM_INCLUDE_PATH})
