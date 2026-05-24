#include "application_state_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk {
ApplicatiionStateQSettings::ApplicatiionStateQSettings(QObject* parent): ApplicatiionState(parent) {}
ApplicatiionStateQSettings::~ApplicatiionStateQSettings() {}

bool ApplicatiionStateQSettings::getIsServer() const {
  settings->beginGroup(applicatiionStateGroup);
  bool isServer = settings->value(isServerKey, false).toBool();
  settings->endGroup();
  return isServer;
}

void ApplicatiionStateQSettings::setIsServer(bool isServer) {
  settings->beginGroup(applicatiionStateGroup);
  settings->setValue(isServerKey, isServer);
  settings->endGroup();
  emit onIsServerChanged(isServer);
}

bool ApplicatiionStateQSettings::shouldUseBluetooth() const {
  settings->beginGroup(applicatiionStateGroup);
  bool useBluetooth = settings->value(useBluetoothKey, false).toBool();
  settings->endGroup();
  return useBluetooth;
}

void ApplicatiionStateQSettings::setUseBluetooth(bool useBluetooth) {
  settings->beginGroup(applicatiionStateGroup);
  settings->setValue(useBluetoothKey, useBluetooth);
  settings->endGroup();
  emit shouldUseBluetoothChanged(useBluetooth);
}
}
