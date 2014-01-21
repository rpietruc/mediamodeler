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
  
    
Build instructions
------------------

Project was successfully built by the authors on Archlinux OS.
Prerequirements: qt5-base, qt5-multimedia, insight-toolkit, opencv, alsa-lib, libsndfile, qcustomplot-qt5, ffmpeg
Build project using qmake (from the main directory, output files will be put into the 'bin' subdirectory)
Create makefiles and compile:

    qmake qmediamodeler.pro -r INCLUDEPATH+=/usr/include/ITK-4.3
    make -j10

Install (default prefix: /usr/local, defined in config.pri)

    sudo make install

Run application:

    LD_LIBRARY_PATH=/usr/local/lib qmediamodeler

Open some ini file from 'ini' directory to build processing graph...

