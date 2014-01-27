project(QMM_OPENCV)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

set(PICTUREBOXPLOTDESTINATION_SRC
    boxplot.cpp
    pictureboxplotdestination.cpp)
set(PICTUREBOXPLOTDESTINATION_INC
    boxplot.h
    pictureboxplotdestination.h)

set(PICTUREDRAWDESTINATION_SRC
    drawwidget.cpp
    picturedrawdestination.cpp)
set(PICTUREDRAWDESTINATION_INC
    drawwidget.h
    picturedrawdestination.h)

set(QMM_OPENCV_SRC
    pictureframes.cpp   
    ${PICTUREBOXPLOTDESTINATION_SRC}
    pictureclustertransform.cpp
    picturecontourtransform.cpp
    ${PICTUREDRAWDESTINATION_SRC}
    pictureedgedetectiontransform.cpp 
    picturefiledestination.cpp
    picturefilesource.cpp
    picturemorphologytransform.cpp
    pictureregiondetectiontransform.cpp
    picturerenderdestination.cpp
    picturesmoothingtransform.cpp
    picturethresholdtransform.cpp
    picturewatershedtransform.cpp
    videocamerasource.cpp
    videofilesource.cpp)

set(QMM_OPENCV_INC
    pictureframes.h 
    ${PICTUREBOXPLOTDESTINATION_INC}
    pictureclustertransform.h
    picturecontourtransform.h 
    ${PICTUREDRAWDESTINATION_INC}
    pictureedgedetectiontransform.h
    picturefiledestination.h
    picturefilesource.h
    picturemorphologytransform.h
    pictureregiondetectiontransform.h
    picturerenderdestination.h
    picturesmoothingtransform.h
    picturethresholdtransform.h
    picturewatershedtransform.h
    videocamerasource.h
    videofilesource.h)


add_library(pictureframes STATIC pictureframes.cpp pictureframes.h)
target_link_libraries(pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS pictureframes ARCHIVE DESTINATION lib)
install(FILES pictureframes.h DESTINATION ${QMM_INCLUDE_PATH})

add_library(pictureboxplotdestination SHARED ${PICTUREBOXPLOTDESTINATION_SRC} ${PICTUREBOXPLOTDESTINATION_INC})
target_link_libraries(pictureboxplotdestination modelcore pictureframes ${Qt5Widgets_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS pictureboxplotdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(pictureclustertransform SHARED pictureclustertransform.cpp pictureclustertransform.h)
target_link_libraries(pictureclustertransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS pictureclustertransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturecontourtransform SHARED picturecontourtransform.cpp picturecontourtransform.h)
target_link_libraries(picturecontourtransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturecontourtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

qt5_wrap_ui(DRAWWIDGET_FRM_H drawwidget.ui)
add_library(picturedrawdestination SHARED ${PICTUREDRAWDESTINATION_SRC} ${PICTUREDRAWDESTINATION_INC} ${DRAWWIDGET_FRM_H})
target_link_libraries(picturedrawdestination modelcore pictureframes ${Qt5Widgets_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturedrawdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(pictureedgedetectiontransform SHARED pictureedgedetectiontransform.cpp pictureedgedetectiontransform.h)
target_link_libraries(pictureedgedetectiontransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS pictureedgedetectiontransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturefiledestination SHARED picturefiledestination.cpp picturefiledestination.h)
target_link_libraries(picturefiledestination modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturefiledestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturefilesource SHARED picturefilesource.cpp picturefilesource.h)
target_link_libraries(picturefilesource modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturefilesource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturemorphologytransform SHARED picturemorphologytransform.cpp picturemorphologytransform.h)
target_link_libraries(picturemorphologytransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturemorphologytransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(pictureregiondetectiontransform SHARED pictureregiondetectiontransform.cpp pictureregiondetectiontransform.h)
target_link_libraries(pictureregiondetectiontransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS pictureregiondetectiontransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturerenderdestination SHARED picturerenderdestination.cpp picturerenderdestination.h)
target_link_libraries(picturerenderdestination modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturerenderdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturesmoothingtransform SHARED picturesmoothingtransform.cpp picturesmoothingtransform.h)
target_link_libraries(picturesmoothingtransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturesmoothingtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturethresholdtransform SHARED picturethresholdtransform.cpp picturethresholdtransform.h)
target_link_libraries(picturethresholdtransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturethresholdtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(picturewatershedtransform SHARED picturewatershedtransform.cpp picturewatershedtransform.h)
target_link_libraries(picturewatershedtransform modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS picturewatershedtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(videocamerasource SHARED videocamerasource.cpp videocamerasource.h)
target_link_libraries(videocamerasource modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS videocamerasource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(videofilesource SHARED videofilesource.cpp videofilesource.h)
target_link_libraries(videofilesource modelcore pictureframes ${Qt5Core_LIBRARIES} ${OpenCV_LIBS})
install(TARGETS videofilesource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

