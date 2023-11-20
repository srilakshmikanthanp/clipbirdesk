// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/**
 * Component for bonjour installer
 */
function Component() {
  // Constructor for component class Leave
  // This as Empty Ued by Qt Installer
  // and we add functions in prototype
}

/**
 * Creates operations for component
 */
Component.prototype.createOperations = function () {
  // create operations
  component.createOperations();

  // if the system is not windows then return
  if (systemInfo.productType === "windows") {
    // get the Env variable for appdata
    var appData = installer.environmentVariable("APPDATA");

    // Add to system startup
    component.addOperation("CreateShortcut",
      "@TargetDir@/clipbird.exe",
      appData + "/Microsoft/Windows/Start Menu/Programs/Startup/ClipBird.lnk",
      "workingDirectory=@TargetDir@",
      "iconPath=@TargetDir@/assets/logo.png",
      "IconId=0",
      "description=ClipBird"
    );

    // if system is x64 install vc_redist.x64
    if (systemInfo.currentCpuArchitecture === "x86_64") {
      component.addOperation("Execute",
        "@TargetDir@/vc_redist.x64.exe",
        "/install",
        "/passive",
        "/norestart",
        "wait"
      );
    }

    // if system is x86 install vc_redist.x86
    if (systemInfo.currentCpuArchitecture === "x86") {
      component.addOperation("Execute",
        "@TargetDir@/vc_redist.x86.exe",
        "/install",
        "/passive",
        "/norestart",
        "wait"
      );
    }

    // Add start menu shortcut
    component.addOperation("CreateShortcut",
      "@TargetDir@/clipbird.exe",
      "@StartMenuDir@/ClipBird.lnk",
      "workingDirectory=@TargetDir@",
      "iconPath=@TargetDir@/assets/logo.ico",
      "IconId=0",
      "description=ClipBird"
    );
  }
}
