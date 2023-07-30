// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

function Component() {
  // Execute the vcredist.exe installer in TargetDir
  installer.execute(installer.value("TargetDir") + "/vcredist.exe");
}
