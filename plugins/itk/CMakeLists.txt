project(QMM_ITK)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
  include_directories(${ITK_INCLUDE_DIRS})
endif(ITK_FOUND)

find_package(Qt5Core REQUIRED)
if(Qt5Core_FOUND)
  include_directories(${Qt5Core_INCLUDES})
  add_definitions(${Qt5Core_DEFINITIONS})
  set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
  add_definitions(-DQT_PLUGIN)
  add_definitions(-DQT_SHARED)
endif(Qt5Core_FOUND)

set(QMM_ITK_SRC
    imageframes.cpp
    imageaddtransform.cpp
    imageandtransform.cpp
    imagecontourtransform.cpp
    imagedenoisingtransform.cpp
    imagegeneratorsource.cpp
    imagescaletransform.cpp
    imagesmoothingtransform.cpp
    imagecurvatureflowtransform.cpp
    imagethinningtransform.cpp
    imagewatershedtransform.cpp
    imageregiongrowingtransform.cpp
    imageseedpointstransform.cpp)

set(QMM_ITK_INC
    imageframes.h
    imageaddtransform.h
    imageandtransform.h
    imagecontourtransform.h
    imagedenoisingtransform.h
    imagegeneratorsource.h
    imagescaletransform.h
    imagesmoothingtransform.h
    imagecurvatureflowtransform.h
    imagethinningtransform.h
    imagewatershedtransform.h
    imageregiongrowingtransform.h
    imageseedpointstransform.cpp)

add_library(imageframes STATIC imageframes.cpp imageframes.h)
target_link_libraries(imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imageframes ARCHIVE DESTINATION lib)
install(FILES imageframes.h DESTINATION ${QMM_INCLUDE_PATH})

add_library(imageaddtransform SHARED imageaddtransform.cpp imageaddtransform.h)
target_link_libraries(imageaddtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imageaddtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imageandtransform SHARED imageandtransform.cpp imageandtransform.h)
target_link_libraries(imageandtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imageandtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagecontourtransform SHARED imagecontourtransform.cpp imagecontourtransform.h)
target_link_libraries(imagecontourtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imagecontourtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagedenoisingtransform SHARED imagedenoisingtransform.cpp imagedenoisingtransform.h)
target_link_libraries(imagedenoisingtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imagedenoisingtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagegeneratorsource SHARED imagegeneratorsource.cpp imagegeneratorsource.h)
target_link_libraries(imagegeneratorsource modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imagegeneratorsource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagescaletransform SHARED imagescaletransform.cpp imagescaletransform.h)
target_link_libraries(imagescaletransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imagescaletransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagesmoothingtransform SHARED imagesmoothingtransform.cpp imagesmoothingtransform.h)
target_link_libraries(imagesmoothingtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imagesmoothingtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagecurvatureflowtransform SHARED imagecurvatureflowtransform.cpp imagecurvatureflowtransform.h)
target_link_libraries(imagecurvatureflowtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imagecurvatureflowtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagethinningtransform SHARED imagethinningtransform.cpp imagethinningtransform.h)
target_link_libraries(imagethinningtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES})
install(TARGETS imagethinningtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imagewatershedtransform SHARED imagewatershedtransform.cpp imagewatershedtransform.h)
target_link_libraries(imagewatershedtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES} ITKWatersheds)
install(TARGETS imagewatershedtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imageregiongrowingtransform SHARED imageregiongrowingtransform.cpp imageregiongrowingtransform.h)
target_link_libraries(imageregiongrowingtransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES} ITKWatersheds)
install(TARGETS imageregiongrowingtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(imageseedpointstransform SHARED imageseedpointstransform.cpp imageseedpointstransform.h)
target_link_libraries(imageseedpointstransform modelcore imageframes ${Qt5Core_LIBRARIES} ${ITK_LIBRARIES} ITKWatersheds)
install(TARGETS imageseedpointstransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})
