// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "qtclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
QtClipboard::QtClipboard(QObject *parent) : PlatformClipboard(parent) {
  connect(qGuiApp->clipboard(), &QClipboard::changed, this, &PlatformClipboard::changed);
}

void QtClipboard::setMimeData(QMimeData *mime, QClipboard::Mode mode) {
  qGuiApp->clipboard()->setMimeData(mime, mode);
}

void QtClipboard::clear(QClipboard::Mode mode) {
  qGuiApp->clipboard()->clear(mode);
}

const QMimeData *QtClipboard::mimeData(QClipboard::Mode mode) const {
  return qGuiApp->clipboard()->mimeData(mode);
}
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard