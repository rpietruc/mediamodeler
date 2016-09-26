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

Dependencies
------------------

* [CMake 3.6.2](https://cmake.org/)
* [Qt 5.7](https://www.qt.io/)
* [Qwt 6.1.3](http://qwt.sourceforge.net/)
* [alsa-lib 1.1.2](http://alsa-project.org/)
* [libsndfile 1.0.27](http://www.mega-nerd.com/libsndfile/)
* [FFmpeg 3.1.3](https://www.ffmpeg.org/)
* [OpenCV 3.1.0](http://opencv.org/)
* [Armadillo 7.400.2](http://arma.sourceforge.net/)
* [Atlas-Lapack 3.10.3](http://math-atlas.sourceforge.net/)
* [mlpack 2.0.3](http://www.mlpack.org/)
* [QCustomPlot-Qt5 1.3.2](http://qcustomplot.com/)

Optional Dependencies
------------------

* [InsightToolkit-4.10.0](https://itk.org/)
* [Potrace 1.13](http://potrace.sourceforge.net/)

Build instructions
------------------

Project was successfully built by the authors on Archlinux OS.

Build entire project using cmake:

    #git clone qmediamodeler...
    mkdir qmediamodeler-build && cd qmediamodeler-build
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug ../qmediamodeler
    make -j5

To build optional plugins use following cmake switches:

    -DBUILD_ITK_PLUGINS=1
    -DBUILD_POTRACE_PLUGINS=1
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
