#!/bin/bash

# Check if current directory has CMakeLists.txt
if [ ! -f "./CMakeLists.txt" ]; then
  echo "CMakeLists.txt not found in current working directory"
  exit 1
fi

#-------------------------- clipbird package --------------------------#

# Build the clipbird with release configuration
echo "Building clipbird with Release configuration"
cmake -G "Ninja" -B ./build && cmake --build ./build --config Release

# clipbird package data directory
ClipbirdDir="./setup"

# Remove the existing appdir folder
echo "Removing existing appdir folder"
rm -rf $ClipbirdDir

# make the appdir folder
echo "Making appdir folder"
mkdir -p $ClipbirdDir

# clone the appdir folder to setup
echo "Cloning appdir folder to $ClipbirdDir"
cp -r ./appdir/* $ClipbirdDir

# Copy the clipbird to the package directory
echo "Copying ./build/src/Release/clipbird to $ClipbirdDir/usr/bin"
cp ./build/src/clipbird "$ClipbirdDir/usr/bin"

# Run linuxdeploy-x86_64.AppImage to package the clipbird
# see https://github.com/linuxdeploy/linuxdeploy
linuxdeploy-x86_64.AppImage --appdir=$ClipbirdDir --plugin qt --output appimage
