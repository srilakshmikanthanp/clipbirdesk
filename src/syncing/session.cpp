#include "session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
QString Session::getName() const {
  return name;
}

bool Session::operator==(const Session &other) const {
  return name == other.name;
}

bool Session::operator!=(const Session &other) const {
  return !(*this == other);
}
}
