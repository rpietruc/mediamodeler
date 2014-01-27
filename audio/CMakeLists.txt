project(QMM_AUDIO)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
if(Qt5Core_FOUND)
  include_directories(${Qt5Core_INCLUDES})
  include_directories(${Qt5Widgets_INCLUDES})
  include_directories(${Qt5Multimedia_INCLUDES})
  include_directories(${Qt5PrintSupport_INCLUDES})
  add_definitions(${Qt5Core_DEFINITIONS})
  add_definitions(${Qt5Widgets_DEFINITIONS})
  add_definitions(${Qt5Multimedia_DEFINITIONS})
  add_definitions(${Qt5PrintSupport_DEFINITIONS})
  set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
  add_definitions(-DQT_PLUGIN)
  add_definitions(-DQT_SHARED)
endif(Qt5Core_FOUND)

find_library(qwt qwt REQUIRED)
find_library(qcustomplot qcustomplot REQUIRED)

set(SPECTROGRAMDESTINATION_SRC
    spectrogramplot.cpp
    spectrogramwindow.cpp
    spectrogramdestination.cpp)

set(SPECTROGRAMDESTINATION_INC
    spectrogramplot.h
    spectrogramwindow.h
    spectrogramdestination.h)

set(SOUNDDEVICEDESTINATION_SRC
    audiooutputdevice.cpp
    sounddevicedestination.cpp)

set(SOUNDDEVICEDESTINATION_INC
    audiooutputdevice.h
    sounddevicedestination.h)

set(AUDIOFRAMES_SRC
    soundframe.cpp
    filterframe.cpp
    formantframe.cpp
    spectrumframe.cpp)

set(AUDIOFRAMES_INC
    soundframe.h
    filterframe.h
    formantframe.h
    spectrumframe.h)

set(QMM_AUDIO_SRC
    ${AUDIOFRAMES_SRC}
    acoustictubetransform.cpp
    formanttransform.cpp
    ${SOUNDDEVICEDESTINATION_SRC}
    sounddevicesource.cpp
    soundfiltertransform.cpp
    soundlinearpredictiontransform.cpp
    soundplotdestination.cpp
    ${SPECTROGRAMDESTINATION_SRC}
    spectrumplotdestination.cpp
    spectrumtransform.cpp)

set(QMM_AUDIO_INC
    ${AUDIOFRAMES_INC}
    acoustictubetransform.h
    formanttransform.h
    ${SOUNDDEVICEDESTINATION_INC}
    sounddevicesource.h
    soundfiltertransform.h
    soundlinearpredictiontransform.h
    soundplotdestination.h
    ${SPECTROGRAMDESTINATION_INC}
    spectrumplotdestination.h
    spectrumtransform.h)

add_library(audioframes STATIC ${AUDIOFRAMES_SRC} ${AUDIOFRAMES_INC})
target_link_libraries(audioframes ${Qt5Multimedia_LIBRARIES})
install(TARGETS audioframes ARCHIVE DESTINATION lib)
install(FILES ${AUDIOFRAMES_INC} DESTINATION ${QMM_INCLUDE_PATH})

add_library(acoustictubetransform SHARED acoustictubetransform.cpp acoustictubetransform.h)
target_link_libraries(acoustictubetransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS acoustictubetransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(formanttransform SHARED formanttransform.cpp formanttransform.h)
target_link_libraries(formanttransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS formanttransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(sounddevicedestination SHARED ${SOUNDDEVICEDESTINATION_SRC} ${SOUNDDEVICEDESTINATION_INC})
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

qt5_wrap_ui(PLOTDIALOG_FRM_H plotdialog.ui)
add_library(soundplotdestination SHARED soundplotdestination.cpp plotdialog.cpp soundplotdestination.h plotdialog.h ${PLOTDIALOG_FRM_H})
target_link_libraries(soundplotdestination modelcore audioframes ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${qcustomplot})
install(TARGETS soundplotdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(spectrogramdestination SHARED ${SPECTROGRAMDESTINATION_SRC} ${SPECTROGRAMDESTINATION_INC})
target_link_libraries(spectrogramdestination modelcore audioframes ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${qwt})
install(TARGETS spectrogramdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(spectrumplotdestination SHARED spectrumplotdestination.cpp plotdialog.cpp spectrumplotdestination.h plotdialog.h ${PLOTDIALOG_FRM_H})
target_link_libraries(spectrumplotdestination modelcore audioframes ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${qcustomplot})
install(TARGETS spectrumplotdestination LIBRARY DESTINATION ${QMM_PLUGINS_PATH})

add_library(spectrumtransform SHARED spectrumtransform.cpp spectrumtransform.h)
target_link_libraries(spectrumtransform modelcore audioframes ${Qt5Core_LIBRARIES} ${Qt5Multimedia_LIBRARIES})
install(TARGETS spectrumtransform LIBRARY DESTINATION ${QMM_PLUGINS_PATH})
