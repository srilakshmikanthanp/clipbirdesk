#include "ssl_config_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::types {
SslConfig SslConfigFactory::getHostSslConfig() {
  auto appState = srilakshmikanthanp::clipbirdesk::ApplicationFactory::getApplicationState();
  if (!appState->getHostSslConfig().has_value() || !appState->getHostSslConfig()->isCertificateValid()) {
    appState->setHostSslConfig(utility::functions::getQSslConfiguration());
  }
  return appState->getHostSslConfig().value();
}
}
