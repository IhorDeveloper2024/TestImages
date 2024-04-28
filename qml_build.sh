#!/bin/bash

cd compression
$1/qmake -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && /usr/bin/make qmake_all
make
rm Makefile
cd ../qml_plugin

$1/qmake -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && /usr/bin/make qmake_all
make

rm clockplugin_metatypes.json
rm clockplugin_qmltyperegistrations.cpp
rm plugins.qmltypes
rm Makefile

bild_path=$PWD/../../build/qml_plugins
mv $bild_path/imports/FramesPlugin/Frames/qmldir $bild_path/imports/FramesPlugin/qmldir

cd ../ui
make clean
$1/qmake -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && /usr/bin/make qmake_all
make
rm Makefile
