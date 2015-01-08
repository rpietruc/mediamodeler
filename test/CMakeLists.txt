project(QMM_TEST)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

link_directories(${CMAKE_BINARY_DIR})
add_definitions(-DBOOST_TEST_DYN_LINK)

find_package(Armadillo REQUIRED)

add_executable(ellipsemerge_test ellipsemerge_test.cpp)
target_link_libraries(ellipsemerge_test contourapproximation ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#install(TARGETS ellipsemerge_test RUNTIME DESTINATION bin)

add_executable(regiongrowingpcnn_test regiongrowingpcnn_test.cpp)
target_link_libraries(regiongrowingpcnn_test regiongrowingpcnn ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#install(TARGETS regiongrowingpcnn_test RUNTIME DESTINATION bin)

add_executable(multiplechannelpcnn_test multiplechannelpcnn_test.cpp)
target_link_libraries(multiplechannelpcnn_test multiplechannelpcnn ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#install(TARGETS multiplechannelpcnn_test RUNTIME DESTINATION bin)
