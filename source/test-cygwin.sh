#!/bin/bash
mkdir ../build
pushd ../build
cmake ../source &&
make &&
./RenderToBitmap < ../data/head.obj > out.bmp &&
cygstart out.bmp
popd
