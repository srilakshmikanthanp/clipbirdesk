#!/bin/bash

# Check if current directory has CMakeLists.txt
if [ ! -f "./CMakeLists.txt" ]; then
  echo "CMakeLists.txt not found in current working directory"
  exit 1
fi

# Set fast fail option
set -e

#-------------------------- clipbird package --------------------------#

# Build the clipbird with release configuration
echo "Building clipbird with Release configuration"
cmake --preset linux-vcpkg-release
cmake --build --preset linux-vcpkg-release

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

clipbirdBinary="$(pwd)/build/linux-vcpkg-release/src/clipbird"
echo "Copying $clipbirdBinary to $ClipbirdDir/usr/bin"
cp $clipbirdBinary "$ClipbirdDir/usr/bin"

qmlFileDir="$(pwd)/build/linux-vcpkg-release/src/Clipbird"
echo "Copying QML files from $qmlFileDir to $ClipbirdDir/usr/bin/Clipbird"
cp -r $qmlFileDir "$ClipbirdDir/usr/bin/Clipbird"

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
