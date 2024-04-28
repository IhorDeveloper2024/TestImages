#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/../build/lib:/home/igor/TestPacketBook/TestImpl/build/qml_plugins/imports/FramesPlugin/
cd /$PWD/../build/
./compress_image_ui -t "$TARGET_LIBRARY"
