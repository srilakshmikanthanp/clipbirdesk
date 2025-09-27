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

linuxPackageDir="$(pwd)/package/linux"
AppRunScript="$linuxPackageDir/AppRun.sh"
appdir=$linuxPackageDir/appdir
ClipbirdDir="$(pwd)/setup"

echo "Removing existing appdir folder"
rm -rf $ClipbirdDir

echo "Making appdir folder"
mkdir -p $ClipbirdDir

echo "Cloning appdir folder to $ClipbirdDir"
cp -r $appdir/* $ClipbirdDir

clipbirdBinary="$(pwd)/build/src/clipbird"
echo "Copying $clipbirdBinary to $ClipbirdDir/usr/bin"
cp $clipbirdBinary "$ClipbirdDir/usr/bin"

dist="$(pwd)/dist"

echo "Recreating dist folder"
rm -rf $dist && mkdir -p $dist

# Run linuxdeploy-x86_64.AppImage to package the clipbird
# see https://github.com/linuxdeploy/linuxdeploy
cd dist && linuxdeploy-x86_64.AppImage \
  --appdir="$ClipbirdDir" \
  --custom-apprun="$AppRunScript" \
  --plugin qt \
  --output appimage
