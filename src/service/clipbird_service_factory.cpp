#include "clipbird_service_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::service {
Q_GLOBAL_STATIC(ClipbirdService, clipbirdServiceInstance)

ClipbirdService* ClipbirdServiceFactory::getClipbirdService() {
  return clipbirdServiceInstance;
}
}
