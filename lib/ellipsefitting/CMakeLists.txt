project(ellipsefitting)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-fPIC)

find_package(Armadillo REQUIRED)

add_library(ellipsefitting STATIC ellipsefitting.cpp ellipsefitting.h)
target_link_libraries(ellipsefitting ${ARMADILLO_LIBRARIES})
