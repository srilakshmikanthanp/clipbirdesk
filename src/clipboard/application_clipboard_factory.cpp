#include "clipboard/application_clipboard_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
Q_GLOBAL_STATIC(ApplicationClipboard, applicationClipboardInstance)

ApplicationClipboard* ApplicationClipboardFactory::getApplicationClipboard() {
  return applicationClipboardInstance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
