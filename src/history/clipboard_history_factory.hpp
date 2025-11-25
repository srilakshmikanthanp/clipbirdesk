#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include "clipboard_history.hpp"

namespace srilakshmikanthanp::clipbirdesk::history {

/**
 * @brief Factory for ClipboardHistory singleton
 */
struct ClipboardHistoryFactory {
  /**
   * @brief Get the ClipboardHistory singleton instance
   * @return ClipboardHistory* Pointer to singleton instance
   */
  static ClipboardHistory* getClipboardHistory();
};

}  // namespace srilakshmikanthanp::clipbirdesk::history
