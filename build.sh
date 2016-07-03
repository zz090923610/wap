#!/bin/bash

if [ "$1" = 'configure' ]
then
    rm CMakeCache.txt cmake_install.cmake  Makefile
    rm -rf CMakeFiles
    cmake -G "Unix Makefiles"
elif [ "$1" = 'clean' ]
then
    rm CMakeCache.txt cmake_install.cmake mapTest Makefile
    rm -rf CMakeFiles
elif [ "$1" = 'demo' ]
then
    rm CMakeCache.txt cmake_install.cmake  Makefile
    rm -rf CMakeFiles
    cmake -G "Unix Makefiles"
    make
    ./mapTest ./data/demo/Sensor0715122921.txt ./data/demo/Wifi0715122921.txt ./data/demo/start.txt
else
    make
fi