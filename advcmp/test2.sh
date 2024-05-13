#!/bin/bash

SCRIPT_DIR="$(dirname $0)"
BUILD_DIR=$SCRIPT_DIR/build
PLUGIN_DIR=$BUILD_DIR/lib

load_passes=""
for f in $PLUGIN_DIR/lib*.so; do
    load_passes+="-load-pass-plugin $f "
done

clang-17 -O0 -S -emit-llvm -Xclang -disable-O0-optnone -Xclang -disable-llvm-passes -fno-discard-value-names test.c -o test.ll
opt-17 -S -passes='mem2reg' ./test.ll -o input.ll
opt-17 -S $load_passes -passes='print<simple-sccp>' './input.ll' -disable-output 2> test.out