project(QMM_APP)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
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
