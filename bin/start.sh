#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}
./compress_image_ui -t "$1"
