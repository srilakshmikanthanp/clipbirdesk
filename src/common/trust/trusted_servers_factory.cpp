#include "trusted_servers_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
Q_GLOBAL_STATIC(TrustedServersQSettings, trustedServersInstance)

TrustedServers* TrustedServersFactory::getTrustedServers() {
  return trustedServersInstance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::common::trust
