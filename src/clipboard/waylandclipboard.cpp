#include "waylandclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
WaylandClipboard::WaylandClipboard(QObject *parent) : PlatformClipboard(parent) {
  // Wayland does not support signals for clipboard change
}

void WaylandClipboard::setMimeData(QMimeData *mime, QClipboard::Mode mode) {
  qGuiApp->clipboard()->setMimeData(mime, mode);
}

void WaylandClipboard::clear(QClipboard::Mode mode) {
  qGuiApp->clipboard()->clear(mode);
}

const QMimeData *WaylandClipboard::mimeData(QClipboard::Mode mode) const {
  return qGuiApp->clipboard()->mimeData(mode);
}
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
