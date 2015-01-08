project(pulsecoupledneuralnetwork)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenCV REQUIRED)

add_library(pulsecoupledneuralnetwork STATIC pulsecoupledneuralnetwork.c pulsecoupledneuralnetwork.h)
target_link_libraries(pulsecoupledneuralnetwork ${OpenCV_LIBS})

add_library(regiongrowingpcnn STATIC regiongrowingpcnn.c regiongrowingpcnn.h)
target_link_libraries(regiongrowingpcnn ${OpenCV_LIBS})

add_library(multiplechannelpcnn STATIC multiplechannelpcnn.cpp multiplechannelpcnn.h)
target_link_libraries(multiplechannelpcnn ${OpenCV_LIBS})
