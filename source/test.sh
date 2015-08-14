#!/bin/bash
# Compile RenderToBitmap project and launch it, piping
# the output to the feh image viewer
mkdir ../build
pushd ../build
cmake ../source && 
make &&
./RenderToBitmap ../data/head.obj ../data/head_diffuse.bmp | feh --force-aliasing -
popd
