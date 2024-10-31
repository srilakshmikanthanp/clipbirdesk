#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QClipboard>
#include <QGuiApplication>

#include "clipboard/platformclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {

class QtClipboard : public PlatformClipboard {
 public:

  explicit QtClipboard(QObject *parent);
  void setMimeData(QMimeData *mime, QClipboard::Mode mode) override;
  void clear(QClipboard::Mode mode) override;
  const QMimeData *mimeData(QClipboard::Mode mode) const override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard