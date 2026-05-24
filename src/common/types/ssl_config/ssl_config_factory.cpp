#include "ssl_config_factory.hpp"
#include "ssl_config_provider.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::types {
Q_GLOBAL_STATIC(SslConfigProvider, sslConfigProviderInstance)

SslConfig SslConfigFactory::getHostSslConfig() {
  if (!sslConfigProviderInstance->getHostSslConfig().has_value() || !sslConfigProviderInstance->getHostSslConfig()->isCertificateValid()) {
    sslConfigProviderInstance->setHostSslConfig(utility::functions::getQSslConfiguration());
  }
  return sslConfigProviderInstance->getHostSslConfig().value();
}
}
