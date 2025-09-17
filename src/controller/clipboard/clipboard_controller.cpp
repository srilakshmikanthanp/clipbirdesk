#include "clipboard_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
ClipboardController::ClipboardController(QObject *parent) : Controller(parent) {}
ClipboardController::~ClipboardController() = default;

clipboard::ApplicationClipboard &ClipboardController::getClipboard() {
  return m_clipboard;
}
}
