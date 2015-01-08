project(QMM_APP)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system chrono program_options REQUIRED)

find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
  include_directories(${ITK_INCLUDE_DIRS})
endif(ITK_FOUND)

include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Boost_INCLUDE_DIR})

add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(QMM_APP_SRC
    main.cpp
    mainwindow.cpp
    fileguielement.cpp
    guielementlist.cpp
    propertiesbox.cpp)
set(QMM_APP_INC
    mainwindow.h
    fileguielement.h
    guielementbase.h
    guielementlist.h
    propertiesbox.h)
set(QMM_APP_FRM
    mainwindow.ui
    fileguielement.ui
    guielementlist.ui)

qt5_wrap_ui(QMM_APP_FRM_H ${QMM_APP_FRM})
#qt5_wrap_cpp(QMM_APP_FRM_MOC ${QMM_APP_FRM_H})
add_executable(mediamodeler ${QMM_APP_SRC} ${QMM_APP_INC} ${QMM_APP_INI} ${QMM_APP_FRM_H})

link_directories(${CMAKE_BINARY_DIR})
target_link_libraries(mediamodeler ${Qt5Widgets_LIBRARIES} modelcore)

install(TARGETS mediamodeler RUNTIME DESTINATION bin)

add_executable(polarcoordinates polarcoordinates.cpp)
target_link_libraries(polarcoordinates contourapproximation ${Boost_PROGRAM_OPTIONS_LIBRARY})
install(TARGETS polarcoordinates RUNTIME DESTINATION bin)

add_executable(approximatecontour approximatecontour.cpp)
target_link_libraries(approximatecontour contourapproximation ${Boost_PROGRAM_OPTIONS_LIBRARY})
install(TARGETS approximatecontour RUNTIME DESTINATION bin)

add_executable(fitellipse fitellipse.cpp)
target_link_libraries(fitellipse ellipsefitting ${Boost_SYSTEM_LIBRARY} ${Boost_CHRONO_LIBRARY})
install(TARGETS fitellipse RUNTIME DESTINATION bin)

add_executable(drawellipse drawellipse.cpp)
target_link_libraries(drawellipse ${OpenCV_LIBS})
install(TARGETS drawellipse RUNTIME DESTINATION bin)

add_executable(contours2 contours2.cpp)
target_link_libraries(contours2 ${OpenCV_LIBS})
install(TARGETS contours2 RUNTIME DESTINATION bin)

add_executable(regiongrowing regiongrowing.c)
target_link_libraries(regiongrowing regiongrowingpcnn popt ${OpenCV_LIBS})
install(TARGETS regiongrowing RUNTIME DESTINATION bin)
