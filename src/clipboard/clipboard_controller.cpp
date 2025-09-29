#include "clipboard_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
ClipboardController::ClipboardController(QObject *parent) : controller::Controller(parent) {}
ClipboardController::~ClipboardController() = default;

clipboard::ApplicationClipboard &ClipboardController::getClipboard() {
  return m_clipboard;
}
}
