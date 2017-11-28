#!/bin/bash
set -x

MMTK_PATH="../mmtk"
cd $MMTK_PATH
git pull && ./build.py

cd -
git pull
./build/gyp/gyp --depth=. acdc.gyp
BUILDTYPE=Release make
cp $MMTK_PATH/libmmtk.so ./allocators/libmmtk.so
cp $MMTK_PATH/libmmtkc.so ./allocators/libmmtkc.so
export LD_LIBRARY_PATH=./allocators
LD_PRELOAD=./allocators/libmmtk.so ./out/Release/acdc -P mmtk -a -s 4 -S 9 -d 30 -l 1 -L 5 -t 1000000
LD_PRELOAD=./allocators/libmmtkc.so ./out/Release/acdc -P mmtkc -a -s 4 -S 9 -d 30 -l 1 -L 5 -t 1000000
LD_PRELOAD=./allocators/libjemalloc.so ./out/Release/acdc -P jemalloc -a -s 4 -S 9 -d 30 -l 1 -L 5 -t 1000000
LD_PRELOAD=./allocators/libhoard.so ./out/Release/acdc -P hoard -a -s 4 -S 9 -d 30 -l 1 -L 5 -t 1000000