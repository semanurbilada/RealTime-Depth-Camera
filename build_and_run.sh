#!/bin/bash

# Exit immediately if any command fails
set -e

echo "Creating build directory if not exists..."
mkdir -p ./build

echo "Compiling the project..."
g++ -std=c++11 -o ./build/depth_estimate ./src/depth_estimate.cpp `pkg-config --cflags --libs opencv4`

echo "Build successful. Running the application..."
./build/depth_estimate