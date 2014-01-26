project(QMM_AUDIO)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Multimedia_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Multimedia_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#    spectrogramwindow.cpp
#    spectrogramwindow.h
#    spectrogramplot.cpp
#    spectrogramplot.h
#    filterconverter.h
#    plotdialog.cpp
#    plotdialog.h
#    plotdialog.ui
#    audiooutputdevice.cpp
#    audiooutputdevice.h

set(QMM_AUDIO_FRM_SRC
    soundframe.cpp
    filterframe.cpp
    formantframe.cpp
    spectrumframe.cpp)

set(QMM_AUDIO_FRM_INC
    soundframe.h
    filterframe.h
    formantframe.h
    spectrumframe.h)

set(QMM_AUDIO_SRC
    ${QMM_AUDIO_FRM_SRC}
    acoustictubetransform.cpp
    formanttransform.cpp
    sounddevicedestination.cpp
    sounddevicesource.cpp
    soundfiltertransform.cpp
    soundlinearpredictiontransform.cpp
    soundplotdestination.cpp
    spectrogramdestination.cpp
    spectrumplotdestination.cpp
    spectrumtransform.cpp)

set(QMM_AUDIO_INC
    ${QMM_AUDIO_FRM_INC}
    acoustictubetransform.h
    formanttransform.h
    sounddevicedestination.h
    sounddevicesource.h
    soundfiltertransform.h
    soundlinearpredictiontransform.h
    soundplotdestination.h
    spectrogramdestination.h
    spectrumplotdestination.h
    spectrumtransform.h)

add_library(audioframes STATIC ${QMM_AUDIO_FRM_SRC} ${QMM_AUDIO_FRM_INC})
target_link_libraries(audioframes ${Qt5Multimedia_LIBRARIES})
install(TARGETS audioframes ARCHIVE DESTINATION lib)

add_library(acoustictubetransform SHARED acoustictubetransform.cpp acoustictubetransform.h)
target_link_libraries(acoustictubetransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS acoustictubetransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(formanttransform SHARED formanttransform.cpp formanttransform.h)
target_link_libraries(formanttransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS formanttransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(sounddevicedestination SHARED sounddevicedestination.cpp sounddevicedestination.h)
target_link_libraries(sounddevicedestination modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS sounddevicedestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(sounddevicesource SHARED sounddevicesource.cpp sounddevicesource.h)
target_link_libraries(sounddevicesource modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS sounddevicesource LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(soundfiltertransform SHARED soundfiltertransform.cpp soundfiltertransform.h)
target_link_libraries(soundfiltertransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS soundfiltertransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(soundlinearpredictiontransform SHARED soundlinearpredictiontransform.cpp soundlinearpredictiontransform.h)
target_link_libraries(soundlinearpredictiontransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS soundlinearpredictiontransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(soundplotdestination SHARED soundplotdestination.cpp soundplotdestination.h)
target_link_libraries(soundplotdestination modelcore audioframes ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS soundplotdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(spectrogramdestination SHARED spectrogramdestination.cpp spectrogramdestination.h)
target_link_libraries(spectrogramdestination modelcore audioframes ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS spectrogramdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(spectrumplotdestination SHARED spectrumplotdestination.cpp spectrumplotdestination.h)
target_link_libraries(spectrumplotdestination modelcore audioframes ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS spectrumplotdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(spectrumtransform SHARED spectrumtransform.cpp spectrumtransform.h)
target_link_libraries(spectrumtransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS spectrumtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})
