#pragma once

#include "common/types/ssl_config/ssl_config.hpp"
#include "utility/functions/ssl/ssl.hpp"
#include "application_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::types {
struct SslConfigFactory {
  static SslConfig getHostSslConfig();
};
}  // namespace srilakshmikanthanp::clipbirdesk::common::types
