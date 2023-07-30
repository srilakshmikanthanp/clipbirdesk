# Copyright (c) 2023 Sri Lakshmi Kanthan P
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

# Usage: kdetools.ps1 <path to qt> <path to bonjour sdk>

# check the arguments
if ($args.Length -ne 2) {
  Write-Host "Usage: kdetools.ps1 <path to qt> <path to bonjour sdk>"
  exit 1
}

# get the path to qt & bonjour sdk
$qt = $args[0]
$bonjour = $args[1]

# Remove .kdetools if exist
if (Test-Path .kdetools) {
  Remove-Item .kdetools -Recurse -Force
}

# make workdir
mkdir .kdetools ; Set-Location .kdetools

# clone the extra-cmake-modules & install it
git clone https://github.com/KDE/extra-cmake-modules.git
Set-Location extra-cmake-modules
cmake -G "Visual Studio 17 2022" -B ./build -DCMAKE_INSTALL_PREFIX="C:\kderoot" -DQT_MAJOR_VERSION=6 -DCMAKE_PREFIX_PATH="${qt};${bonjour}"
cmake --build ./build --config Release
Set-Location build
msbuild ECM.sln
msbuild INSTALL.vcxproj

# Back to root
Set-Location ..\..

# clone the kdnssd & install it
git clone https://invent.kde.org/srilakshmikanthanp/kdnssd.git
Set-Location kdnssd
cmake -G "Visual Studio 17 2022" -B ./build -DCMAKE_INSTALL_PREFIX="C:\kderoot" -DQT_MAJOR_VERSION=6 -DCMAKE_PREFIX_PATH="${qt};${bonjour}"
cmake --build ./build --config Release
Set-Location build
msbuild kdnssd.sln
msbuild INSTALL.vcxproj

# Add the kderoot to path & KDNSSD_DIR to env
$env:KDEROOT = "C:\kderoot"
$env:ECM_DIR = "C:\kderoot\share\ECM\cmake"
$env:PATH = "C:\kderoot\bin;$env:PATH"
$env:KDNSSD_DIR = "C:\kderoot\lib\cmake\KF5KDNSSD"

# remove the workdir Recursively
Set-Location ..\..\.. -and Remove-Item .kdetools -Recurse -Force
