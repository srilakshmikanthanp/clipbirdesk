#pragma once

#include "clipbird_service.hpp"

namespace srilakshmikanthanp::clipbirdesk::service {
struct ClipbirdServiceFactory {
  static ClipbirdService* getClipbirdService();
};
}
