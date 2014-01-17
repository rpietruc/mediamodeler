mediamodeler - Multidimensional Signal/Image Processing Framework 
=================================================================

Introduction
------------

This project contains sources of mediamodeler - a framework facilitating the development of audio/video processing algorithms.
mediamodeler is written in C++ using Qt library, composing of a core library with data processing plugins and gui controller.
The data is streamed within graph consisted of 3 types of elements: sources, transforms and destinations.
The abstract class of the data frame can have any number of dimensions and resolution of each dimension.

This software is distributed under license:

* MIT license

Main author:

* Rafal Pietruch
  
    
Build instructions
------------------

Build project using qmake (from the main directory, binaries will be put into the 'bin' subdirectory)
Create makefiles e.g:
    qmake mediamodeler.pro -r -spec linux-g++ CONFIG+=debug INCLUDEPATH+=/usr/include/qt INCLUDEPATH+=/usr/include/ITK-4.3
    make -j10

Run application using libraries, plugins from 'bin' subdirectory
    cd bin
    LD_LIBRARY_PATH=. ./app

