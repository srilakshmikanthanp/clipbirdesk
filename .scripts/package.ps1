# Copyright (c) 2023 Sri Lakshmi Kanthan P
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT


# if is not Developer Command Prompt for VS 2022
if ($env:VSCMD_ARG_app_plat -ne  "Desktop") {
  Write-Host "Please run this script in Developer Command Prompt for VS 2022" -ForegroundColor Red
  exit 1
}

# Check if current directory has CMakeLists.txt
if (-not (Test-Path ./CMakeLists.txt)) {
  Write-Host "CMakeLists.txt not found in current working directory" -ForegroundColor Red
  exit 1
}

#-------------------------- clipbird package --------------------------#

# Buil the clipbird with release configuration
Write-Host "Building clipbird with Release configuration" -ForegroundColor Green
cmake -G "Visual Studio 17 2022" -B ./build && cmake --build ./build --config Release

# clipbird package data directory
$ClipbirDir = "./.installer/packages/clipbird/data"

# clean the package directory items except .gitignore
Write-Host "Cleaning the package directory $ClipbirDir" -ForegroundColor Green
Remove-Item -Recurse -Force $ClipbirDir/* -Exclude .gitignore

# Copy All openssl dlls to the package directory
Write-Host "Copying $env:OPENSSL_ROOT_DIR /bin/*.dll to $ClipbirDir" -ForegroundColor Green
Copy-Item "$env:OPENSSL_ROOT_DIR/bin/*.dll" $ClipbirDir

# create assets directory in the package directory
Write-Host "Creating assets directory in $ClipbirDir" -ForegroundColor Green
New-Item -ItemType Directory -Force -Path $ClipbirDir/assets/images

# copy the Logo to the package directory
Write-Host "Copying ./assets/images/* to $ClipbirDir" -ForegroundColor Green
Copy-Item ./assets/images/* $ClipbirDir/assets/images

# Create the package as BuildType (to lower) version
Write-Host "Creating the package as Release version" -ForegroundColor Green
windeployqt ./build/Release/clipbird.exe --dir $ClipbirDir --release

# copy the clipbird.exe to the package directory
Write-Host "Copying ./build/Release/clipbird.exe to $ClipbirDir" -ForegroundColor Green
Copy-Item ./build/Release/clipbird.exe $ClipbirDir

#------------------ qt installer framework -----------------------#

# Config file for qt installer framework
$ConfigFile = "./.installer/configuration/config.xml"

# package directory for qt installer framework
$PackageDir = "./.installer/packages"

# destination directory for qt installer framework
$Destination = "./build/installer"

# make sure that the destination directory exists
New-Item -ItemType Directory -Force -Path $Destination

# Clean the destination directory
Write-Host "Cleaning the $Destination file" -ForegroundColor Green
Remove-Item -Recurse -Force $Destination/*

# create binary package with qt installer framework
Write-Host "Creating binary package... " -ForegroundColor Green
binarycreator -c "$ConfigFile" -p "$PackageDir"  "$Destination/clipbird.exe"
