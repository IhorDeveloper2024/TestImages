#!/bin/bash

#TODO Need to set own paths!!
QT_BIN_PATH='/home/igor/Qt/6.6.1/gcc_64/bin/'
CMAKE_BIN_PATH="/home/igor/Qt/6.6.1/gcc_64/lib/cmake/" 
TARGET_LIBRARY="/home/igor/TestPacketBook/Тестове завдання С++ PocketBook/"


echo "----------------------------------------------------------------"
echo "If build is failed yuoy need to update paths:"
echo "QT_BIN_PATH - path to qmake"
echo "CMAKE_BIN_PATH - path to cmake"
echo "TARGET_LIBRARY - path to images folder"
echo "----------------------------------------------------------------"

USE_QMAKE='yes'
if [ "$1" = "" ]; then
	echo "Use QMake for build"
	USE_QMAKE='yes'
else
	echo "Use CMake for build"
	USE_QMAKE='no'
fi

if [ "$USE_QMAKE" = 'yes' ];
then
    echo "Start Qmake build"
    ./qml_build.sh $QT_BIN_PATH
cd ../build/
else
    ./cmake_build.sh $CMAKE_BIN_PATH $QT_BIN_PATH
fi
./cmake_build.sh 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/../build/lib
cd /$PWD/../build/
./compress_image_ui -t "$TARGET_LIBRARY"
