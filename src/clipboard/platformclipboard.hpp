#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>
#include <QObject>

namespace srilakshmikanthanp::clipbirdesk::clipboard {
class PlatformClipboard : public QObject {
 public:

  /**
   * Sets the clipboard to the new contents
   * The clipboard takes ownership of mime
   */
  virtual void setMimeData(QMimeData *mime, QClipboard::Mode mode) = 0;

  /**
   * Clears the current clipboard
   */
  virtual void clear(QClipboard::Mode mode)                        = 0;

  /**
   * Returns the current mime data received by the clipboard
   */
  virtual const QMimeData *mimeData(QClipboard::Mode mode) const   = 0;

  /**
   * Returns the text content of the Clipboard
   *
   * Similar to QClipboard::text(QClipboard::Mode mode)
   */
  QString text(QClipboard::Mode mode);

  /**
   * Returns a shared global PlatformClipboard instance
   */
  static PlatformClipboard *instance();

 signals:
  /**
   * Emitted when the clipboard changes similar to QClipboard::changed
   */
  void changed(QClipboard::Mode mode);

 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 protected:

  PlatformClipboard(QObject *parent);
};
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
