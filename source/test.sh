#!/bin/bash
# Compile RenderToBitmap project and launch it, piping
# the output to the feh image viewer
mkdir ../build
pushd ../build
cmake ../source && 
make &&
./RenderToBitmap < ../data/head.obj | feh --force-aliasing -
popd
