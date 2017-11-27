#!/bin/bash
set -x

MMTK_PATH="../mmtk"
cd $MMTK_PATH
git pull && ./build.py

cd -
./build/gyp/gyp --depth=. acdc.gyp
BUILDTYPE=Release make
cp $MMTK_PATH/libmmtk.so ./allocators/libmmtk.so
cp $MMTK_PATH/libmmtkc.so ./allocators/libmmtkc.so
export LD_LIBRARY_PATH=./allocators
LD_PRELPOAD=./allocators/libmmtk.so ./out/Release/acdc -P mmtk
LD_PRELPOAD=./allocators/libmmtkc.so ./out/Release/acdc -P mmtkc