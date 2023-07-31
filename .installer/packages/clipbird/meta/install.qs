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
  // if the system is not windows then return
  if (systemInfo.productType !== "windows") {
    return;
  }

  // create component
  component.createOperations();

  // get the Env variable for appdata
  var appData = installer.environmentVariable("APPDATA");

  // Add to system startup
  component.addOperation("CreateShortcut",
    "@TargetDir@/clipbird.exe",
    appData + "/Microsoft/Windows/Start Menu/Programs/Startup/ClipBird.lnk",
    "workingDirectory=@TargetDir@",
    "iconPath=@TargetDir@/logo.png"
  );

  // Add start menu shortcut
  component.addOperation("CreateShortcut",
    "@TargetDir@/clipbird.exe",
    "@StartMenuDir@/ClipBird.lnk",
    "workingDirectory=@TargetDir@",
    "iconPath=@TargetDir@/logo.png"
  );
}
