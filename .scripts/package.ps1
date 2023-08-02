# Copyright (c) 2023 Sri Lakshmi Kanthan P
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

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
Write-Host "Copying $env:OPENSSL_DIR/bin/*.dll to $ClipbirDir" -ForegroundColor Green
Copy-Item "$env:OPENSSL_DIR/bin/*.dll" $ClipbirDir

# copy the Logo to the package directory
Write-Host "Copying ./assets/images/logo.png to $ClipbirDir" -ForegroundColor Green
Copy-Item ./assets/images/logo.png $ClipbirDir

# Create the package as BuildType (to lower) version
Write-Host "Creating the package as Release version" -ForegroundColor Green
windeployqt ./build/Release/clipbird.exe --dir $ClipbirDir --release

# copy the clipbird.exe to the package directory
Write-Host "Copying ./build/Release/clipbird.exe to $ClipbirDir" -ForegroundColor Green
Copy-Item ./build/Release/clipbird.exe $ClipbirDir

#-------------------------- bonjour package --------------------------#

# Bonjour Package data directory
$BonjourDir = "./.installer/packages/bonjour/data"

# clean the package directory items except .gitignore
Write-Host "Cleaning the package directory $BonjourDir" -ForegroundColor Green
Remove-Item -Recurse -Force $BonjourDir/* -Exclude .gitignore

# copy the bonjour installer to the package if x64
if ($env:PROCESSOR_ARCHITECTURE -eq "AMD64") {
  $bonjour_installer = "bonjour64.msi"
} else {
  $bonjour_installer = "bonjour.msi"
}

# copy the bonjour installer to the package x86
Write-Host "Copying $bonjour_installer to $BonjourDir" -ForegroundColor Green
Copy-Item "$env:BONJOUR_SDK_HOME/Installer/$bonjour_installer" $BonjourDir

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
