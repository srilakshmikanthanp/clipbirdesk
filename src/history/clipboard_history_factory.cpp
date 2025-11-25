#include "clipboard_history_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::history {

Q_GLOBAL_STATIC(ClipboardHistory, clipboardHistoryInstance)

ClipboardHistory* ClipboardHistoryFactory::getClipboardHistory() {
  return clipboardHistoryInstance;
}

}  // namespace srilakshmikanthanp::clipbirdesk::history
