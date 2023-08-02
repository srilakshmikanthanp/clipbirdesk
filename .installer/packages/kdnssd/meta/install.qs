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

  // if the system is linux
  if (systemInfo.productType === "linux") {
    // install kdnssd-kf5 package for runtime
    component.addOperation(
      "Execute",
      "pkexec",
      "apt-get",
      ["install", "-y", "libkf5dnssd5"],
      "installer.execute"
    );
  }
}
