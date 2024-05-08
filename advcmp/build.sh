#!/bin/bash

SCRIPT_DIR="$(dirname $0)"

cmake -S $SCRIPT_DIR/project1 -G Ninja -B build
cmake --build build