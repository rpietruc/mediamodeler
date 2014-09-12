project(QMM_TEST)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR} "../plugins/opencv")

link_directories(${CMAKE_BINARY_DIR})
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(regiongrowingpcnn_test regiongrowingpcnn_test.cpp)
target_link_libraries(regiongrowingpcnn_test regiongrowingpcnn ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
#install(TARGETS regiongrowingpcnn_test RUNTIME DESTINATION bin)
