#pragma once

#include "common/trust/trusted_clients.hpp"
#include "common/trust/trusted_clients_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
struct TrustedClientsFactory {
  static TrustedClients* getTrustedClients();
};
}
