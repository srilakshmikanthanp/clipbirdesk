:: Copyright (c) 2023 Sri Lakshmi Kanthan P
::
:: This software is released under the MIT License.
:: https://opensource.org/licenses/MIT

:: Usage: kdetools.bat <path to qt> <path to bonjour sdk>

:: check the arguments
if "%1" == "" goto :error
if "%2" == "" goto :error

:: make workdir
mkdir .kdetools && cd .kdetools

:: clone the extra-cmake-modules & install it
git clone https://github.com/KDE/extra-cmake-modules.git
cd extra-cmake-modules
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX="C:\kderoot" -DQT_MAJOR_VERSION=6 -DCMAKE_PREFIX_PATH="%1;%2" -G "Visual Studio 17 2022" -S ..
msbuild ECM.sln
msbuild INSTALL.vcxproj

:: Back to root
cd ..\..

:: clone the kdnssd & install it
git clone https://invent.kde.org/srilakshmikanthanp/kdnssd.git
cd kdnssd
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX="C:\kderoot" -DQT_MAJOR_VERSION=6 -DCMAKE_PREFIX_PATH="%1;%2" -G "Visual Studio 17 2022" -S ..
msbuild kdnssd.sln
msbuild INSTALL.vcxproj

:: remove the workdir
cd ..\.. && rmdir /s /q .kdetools
