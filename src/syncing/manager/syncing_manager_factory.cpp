#include "syncing_manager_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
Q_GLOBAL_STATIC(SyncingManager, syncingManagerInstance)

SyncingManager* SyncingManagerFactory::getSyncingManager() {
  return syncingManagerInstance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
