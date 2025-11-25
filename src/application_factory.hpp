#pragma once

#include "application_state_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk {
struct ApplicationFactory {
  static ApplicatiionState* getApplicationState();
};
}  // namespace clipbirdesk
