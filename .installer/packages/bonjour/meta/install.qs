// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

function Component() {
  // Execute the bonjour.msi installer
  installer.execute("msiexec", ["/i", installer.value("TargetDir") + "/bonjour.msi", "/qb"]);
}
