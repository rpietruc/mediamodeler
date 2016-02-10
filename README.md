qmediamodeler - Multidimensional Signal/Image Processing Framework 
==================================================================

Introduction
------------

This project contains sources of qmediamodeler - a framework facilitating the development of audio/video processing algorithms.
Qmediamodeler is written in C++ using Qt library, composing of a core library with data processing plugins and gui controller.
The data is streamed within graph consisted of 3 types of elements: sources, transforms and destinations.
The abstract class of the data frame can have any number of dimensions and resolution of each dimension.

This software is distributed under license:

* MIT license

Main authors:

* Rafal Pietruch

Prerequisites
------------------

* CMake 2.8+
* Qt5 5.5 (base, multimedia)
* Qwt 6.1.2 (Qt5)
* alsa-lib 1.1.0
* libsndfile 1.0.26
* FFmpeg 2.8.6
* OpenCV 2.4.12
* InsightToolkit-4.9.0
* armadillo 6.500.4
* mlpack 2.0.1
* QCustomPlot 1.3.2 (Qt5)
 
Build instructions
------------------

Project was successfully built by the authors on Archlinux OS.

Build entire project using cmake:

    #git clone qmediamodeler...
    mkdir qmediamodeler-build && cd qmediamodeler-build
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug ../qmediamodeler
    make -j5

To build only specific plugins use following cmake switches:

    -DBUILD_ALSA_PLUGINS=1
    -DBUILD_AV_PLUGINS=1
    -DBUILD_AUDIO_PLUGINS=1
    -DBUILD_ITK_PLUGINS=1
    -DBUILD_OPENCV_PLUGINS=1
    -DBUILD_POTRACE_PLUGINS=1
    -DBUILD_ARMADILLO_PLUGINS=1
    -DBUILD_MLPACK_PLUGINS=1
    -DBUILD_OTB_PLUGINS=1

Install:

    sudo make install

Run application:

    mediamodeler

Open some ini file from 'ini' directory to build processing graph...

Example test data
-----------------

Speech Codec Wav Samples:

    http://www.signalogic.com/index.pl?page=codec_samples
