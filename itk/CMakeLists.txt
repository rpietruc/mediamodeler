project(QMM_ITK)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(ITK_LIBS ITKCommon-4.3 itkvnl-4.3)
find_package(Qt5Core REQUIRED)
find_package(ITK REQUIRED)
include_directories(${Qt5Core_INCLUDES})
include_directories("/usr/include/ITK-4.3")
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

set(QMM_ITK_SRC
    imageframes.cpp   
    imageaddtransform.cpp
    imageandtransform.cpp
    imagedenoisingtransform.cpp 
    imagegeneratorsource.cpp
    imagescaletransform.cpp
    imagesmoothingtransform.cpp
    imagethinningtransform.cpp)

set(QMM_ITK_INC
    imageframes.h 
    imageaddtransform.h
    imageandtransform.h 
    imagedenoisingtransform.h
    imagegeneratorsource.h
    imagescaletransform.h
    imagesmoothingtransform.h
    imagethinningtransform.h)


add_library(imageframes STATIC imageframes.cpp imageframes.h)
target_link_libraries(imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imageframes ARCHIVE DESTINATION lib)
install(FILES imageframes.h DESTINATION ${QMM_INCLUDE_PATH})

add_library(imageaddtransform SHARED imageaddtransform.cpp imageaddtransform.h)
target_link_libraries(imageaddtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imageaddtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imageandtransform SHARED imageandtransform.cpp imageandtransform.h)
target_link_libraries(imageandtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imageandtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagedenoisingtransform SHARED imagedenoisingtransform.cpp imagedenoisingtransform.h)
target_link_libraries(imagedenoisingtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imagedenoisingtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagegeneratorsource SHARED imagegeneratorsource.cpp imagegeneratorsource.h)
target_link_libraries(imagegeneratorsource modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imagegeneratorsource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagescaletransform SHARED imagescaletransform.cpp imagescaletransform.h)
target_link_libraries(imagescaletransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imagescaletransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagesmoothingtransform SHARED imagesmoothingtransform.cpp imagesmoothingtransform.h)
target_link_libraries(imagesmoothingtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imagesmoothingtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagethinningtransform SHARED imagethinningtransform.cpp imagethinningtransform.h)
target_link_libraries(imagethinningtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBS})
install(TARGETS imagethinningtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

