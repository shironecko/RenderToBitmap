#!/bin/bash
mkdir ../build
pushd ../build
cmake ../source &&
make &&
./RenderToBitmap ../data/head.obj ../data/head_diffuse.bmp > out.bmp &&
cygstart out.bmp
popd
