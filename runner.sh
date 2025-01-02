#!/bin/bash

echo "Installing required dependencies"

sudo apt-get update
sudo apt-get install -y git cmake make g++ libgtk2.0-dev pkg-config build-essential

echo "Cloning OpenCV repository"
git clone --depth 1 "https://github.com/opencv/opencv.git"

echo "Building OpenCV"
cd opencv
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)

echo "Installing OpenCV"
sudo make install
sudo ldconfig

echo "Building application"
make

echo "Build complete!"
echo "Application written to bin/main"