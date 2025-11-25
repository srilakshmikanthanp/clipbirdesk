#include "application_state_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk {
ApplicatiionStateQSettings::ApplicatiionStateQSettings(QObject* parent): ApplicatiionState(parent) {}
ApplicatiionStateQSettings::~ApplicatiionStateQSettings() {}

void ApplicatiionStateQSettings::setHostSslConfig(const std::optional<common::types::SslConfig>& sslConfig) {
  settings->beginGroup(applicatiionStateGroup);
  if (sslConfig.has_value()) {
    settings->setValue(certificateKey, sslConfig->certificate);
    settings->setValue(keyKey, sslConfig->privateKey);
  } else {
    settings->remove(certificateKey);
    settings->remove(keyKey);
  }
  settings->endGroup();
  emit onHostSSlConfigChanged(sslConfig);
}

std::optional<common::types::SslConfig> ApplicatiionStateQSettings::getHostSslConfig() const {
  settings->beginGroup(applicatiionStateGroup);
  QByteArray certificate = settings->value(certificateKey).toByteArray();
  QByteArray key = settings->value(keyKey).toByteArray();
  settings->endGroup();
  if (certificate.isEmpty() || key.isEmpty()) {
    return std::nullopt;
  }
  return common::types::SslConfig{.privateKey = key, .certificate = certificate};
}

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
