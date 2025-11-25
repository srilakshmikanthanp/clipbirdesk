#include "trusted_clients_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
Q_GLOBAL_STATIC(TrustedClientsQSettings, trustedClientsInstance)

TrustedClients* TrustedClientsFactory::getTrustedClients() {
  return trustedClientsInstance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::common::trust
