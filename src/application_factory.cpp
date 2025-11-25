#include "application_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk {
Q_GLOBAL_STATIC(ApplicatiionStateQSettings, applicationStateInstance)

ApplicatiionState* ApplicationFactory::getApplicationState() {
  return applicationStateInstance;
}
}  // namespace clipbirdesk
