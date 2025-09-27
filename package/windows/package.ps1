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

Write-Host "Building clipbird with Release configuration" -ForegroundColor Green
cmake -G "Visual Studio 17 2022" -B ./build && cmake --build ./build --config Release

$ClipbirDir = "./setup"

Write-Host "Creating the package directory $ClipbirDir" -ForegroundColor Green
New-Item -ItemType Directory -Force -Path $ClipbirDir

Write-Host "Cleaning the package directory $ClipbirDir" -ForegroundColor Green
Remove-Item -Recurse -Force $ClipbirDir/*

Write-Host "Copying $env:BONJOUR_SDK_HOME/Installer/Bonjour64.msi to $ClipbirDir" -ForegroundColor Green
Copy-Item "$env:BONJOUR_SDK_HOME/Installer/Bonjour64.msi" $ClipbirDir

Write-Host "Copying $env:OPENSSL_ROOT_DIR /bin/*.dll to $ClipbirDir" -ForegroundColor Green
Copy-Item "$env:OPENSSL_ROOT_DIR/bin/*.dll" $ClipbirDir

Write-Host "Creating the package as Release version" -ForegroundColor Green
windeployqt ./build/src/Release/clipbird.exe --dir $ClipbirDir --release

Write-Host "Copying ./build/Release/clipbird.exe to $ClipbirDir" -ForegroundColor Green
Copy-Item ./build/src/Release/clipbird.exe $ClipbirDir
