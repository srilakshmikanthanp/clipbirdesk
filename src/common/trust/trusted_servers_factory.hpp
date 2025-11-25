#pragma once

#include "common/trust/trusted_servers.hpp"
#include "common/trust/trusted_servers_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
struct TrustedServersFactory {
  static TrustedServers* getTrustedServers();
};
}  // namespace srilakshmikanthanp::clipbirdesk::common::trust
