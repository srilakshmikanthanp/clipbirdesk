// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "waylandclipboard.hpp"

#include <qprocess.h>

namespace srilakshmikanthanp::clipbirdesk::clipboard {
WaylandClipboard::WaylandClipboard(QObject *parent) : PlatformClipboard(parent) {}

void WaylandClipboard::setMimeData(QMimeData *mime, QClipboard::Mode mode) {
  // TODO
}

void WaylandClipboard::clear(QClipboard::Mode mode) {
  // TODO
}

const QMimeData *WaylandClipboard::mimeData(QClipboard::Mode mode) const {
  // TODO
}
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
