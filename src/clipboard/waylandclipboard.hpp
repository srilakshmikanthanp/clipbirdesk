#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include "clipboard/platformclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {

class WaylandClipboard : public PlatformClipboard {
 public:

  WaylandClipboard(QObject *parent);
  void setMimeData(QMimeData *mime, QClipboard::Mode mode) override;
  void clear(QClipboard::Mode mode) override;
  const QMimeData *mimeData(QClipboard::Mode mode) const override;
};

}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
