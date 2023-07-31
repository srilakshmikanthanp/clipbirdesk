// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/**
 * Component for bonjour installer
 */
function Component() {
  // Constructor for component class Leave
  // This as Empty Used by Qt Installer
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

  // install bonjour
  component.addOperation("Execute",
    "msiexec",
    "/i",
    "@TargetDir@/Bonjour.msi",
    "/qn",
    "REINSTALL=ALL",
    "REINSTALLMODE=vomus",
    "ARPSYSTEMCOMPONENT=1",
    "ARPNOREPAIR=1",
    "ARPNOMODIFY=1",
    "ARPNOREMOVE=1"
  );
}
