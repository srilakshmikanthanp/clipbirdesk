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

AppRunScript="$(pwd)/linux/AppRun.sh"
ClipbirdDir="$(pwd)/setup"

echo "Removing existing appdir folder"
rm -rf $ClipbirdDir

echo "Making appdir folder"
mkdir -p $ClipbirdDir

echo "Cloning appdir folder to $ClipbirdDir"
cp -r ./linux/appdir/* $ClipbirdDir

echo "Copying ./build/src/Release/clipbird to $ClipbirdDir/usr/bin"
cp ./build/src/clipbird "$ClipbirdDir/usr/bin"

echo "Removing existing dist folder"
rm -rf ./dist

echo "Creating dist folder"
mkdir -p ./dist

# Run linuxdeploy-x86_64.AppImage to package the clipbird
# see https://github.com/linuxdeploy/linuxdeploy
cd dist && linuxdeploy-x86_64.AppImage \
  --appdir="$ClipbirdDir" \
  --custom-apprun="$AppRunScript" \
  --plugin qt \
  --output appimage
