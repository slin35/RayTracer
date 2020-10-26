#!/bin/bash

mkdir -p build
cd build
cmake ../
make
mkdir -p ../output
./raytrace 640 480 ../input/p3_e4.pov ../output/out.ppm 1 100 50

# for Mac OSX
if [[ "$OSTYPE" == "darwin"* ]]; then
    open ../output/out.ppm
fi