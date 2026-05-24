#include "ssl_config_provider.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::types {
SslConfigProvider::SslConfigProvider(QObject* parent) : QObject(parent) {}

SslConfigProvider::~SslConfigProvider() {}

void SslConfigProvider::setHostSslConfig(const std::optional<common::types::SslConfig>& sslConfig) {
  settings->beginGroup(sslConfigGroup);
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

std::optional<common::types::SslConfig> SslConfigProvider::getHostSslConfig() const {
  settings->beginGroup(sslConfigGroup);
  QByteArray certificate = settings->value(certificateKey).toByteArray();
  QByteArray key = settings->value(keyKey).toByteArray();
  settings->endGroup();
  if (certificate.isEmpty() || key.isEmpty()) return std::nullopt;
  return common::types::SslConfig{.privateKey = key, .certificate = certificate};
}
}
