#pragma once

#include "syncing/manager/syncing_manager.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
struct SyncingManagerFactory {
  static SyncingManager* getSyncingManager();
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
