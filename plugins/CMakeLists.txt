project(QMM_PLUGINS)

#mandatory plugins

add_subdirectory(alsa)
add_subdirectory(av)
add_subdirectory(audio)
add_subdirectory(opencv)
add_subdirectory(mlpack)
add_subdirectory(armadillo)

#optional plugins

if(BUILD_ITK_PLUGINS)
add_subdirectory(itk)
endif(BUILD_ITK_PLUGINS)

if(BUILD_POTRACE_PLUGINS)
add_subdirectory(potrace)
endif(BUILD_POTRACE_PLUGINS)

if(BUILD_OTB_PLUGINS)
add_subdirectory(otb)
endif(BUILD_OTB_PLUGINS)
