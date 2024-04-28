#!/bin/bash

HOME_DIR=${PWD}
cd ${HOME_DIR}/qml_plugin
cmake -DCMAKE_PREFIX_PATH=$1 -B$../../build/qml_plugin -DQMAKE_BIN_PATH=$2
mv ${PWD}/../../build/qml_plugins/imports/FramesPlugin/Frames/qmldir ${PWD}/../../build/qml_plugins/imports/FramesPlugin

rm ${PWD}/framesplugin_metatypes.json
rm ${PWD}/framesplugin_qmltyperegistrations.cpp
rm ${PWD}/plugins.qmltypes
rm ${PWD}/Makefile
rm -r build

cd ${HOME_DIR}/compression
cmake -DCMAKE_PREFIX_PATH=$1 -B../../build/lib
cd ${PWD}/../../build/lib
make

cd ${HOME_DIR}/ui
cmake -DCMAKE_PREFIX_PATH=$1 -B../../build/ui
cd ${PWD}/../../build/ui
make

mv compress_image_ui ../
