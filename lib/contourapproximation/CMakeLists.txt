project(contourapproximation)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-fPIC)

find_package(OpenCV REQUIRED)
#find_package(GSL REQUIRED)

set(GSL_LIBRARIES
    gsl
    gslcblas)

set(contourapproximation_sources
    closestpoint.cpp)

set(contourapproximation_includes
    contourapprox.h
    contourpoints.h
    ellipsecontour.h
    linecontour.h
    segmentcontour.h)

add_library(contourapproximation STATIC ${contourapproximation_sources} ${contourapproximation_includes})
target_link_libraries(contourapproximation ellipsefitting ${OpenCV_LIBS} ${GSL_LIBRARIES})
