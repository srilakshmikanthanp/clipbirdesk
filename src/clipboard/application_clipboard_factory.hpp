#pragma once

#include "clipboard/applicationclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
/**
 * @brief Factory for ApplicationClipboard singleton
 */
struct ApplicationClipboardFactory {
  /**
   * @brief Get the ApplicationClipboard singleton instance
   * @return ApplicationClipboard* Pointer to singleton instance
   */
  static ApplicationClipboard* getApplicationClipboard();
};
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
