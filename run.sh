#!/bin/bash

mkdir -p build
cd build
cmake ../
make
mkdir -p ../output
./raytrace 640 480 ../input/P3_r1.pov ../output/out.ppm 1 20 50

# for Mac OSX
if [[ "$OSTYPE" == "darwin"* ]]; then
    open ../output/out.ppm
fi