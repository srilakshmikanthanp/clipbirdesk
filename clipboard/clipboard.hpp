#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QBuffer>
#include <QByteArray>
#include <QClipboard>
#include <QIODevice>
#include <QImage>
#include <QImageReader>
#include <QList>
#include <QMimeData>
#include <QObject>
#include <QPair>
#include <QString>
#include <QUrl>
#include <QVector>

// project header
#include "types/except/except.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
/**
 * @brief Class to manage clipboard such get,
 * set and notify the clipboard change
 */
class Clipboard : public QObject {
 signals:  // signals
  /**
   * @brief Signal to notify the clipboard change occurrence use
   * the parameter or get method to get the clipboard data
   * @param items clipboard data
   */
  void OnClipboardChange(QVector<QPair<QString, QByteArray>>);

 private:  // members

  QClipboard* m_clipboard = nullptr;

 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Clipboard)

 private:  // private slots

  /// @brief Slot to notify the clipboard change
  void onClipboardChangeImpl();

 private:  // mime types

  const QString MIME_TYPE_TEXT  = "text/plain";
  const QString MIME_TYPE_URL   = "text/uri-list";
  const QString MIME_TYPE_PNG   = "image/png";
  const QString MIME_TYPE_COLOR = "application/x-color";
  const QString MIME_TYPE_HTML  = "text/html";

 public:  // constructor

  /**
   * @brief Construct a new Clipboard object and manage
   * the clipboard that is passed via the constructor
   *
   * @param clipboard Clipboard that is managed
   * @param parent parent object
   */
  explicit Clipboard(QClipboard* clipboard, QObject* parent = nullptr);

  /**
   * @brief Get the clipboard data from the clipboard
   *
   * @return mime type and data
   */
  QVector<QPair<QString, QByteArray>> get();

  /**
   * @brief Clear the clipboard content
   */
  void clear();

  /**
   * @brief Set the clipboard data to the clipboard
   *
   * @param mime mime type of the data
   * @param data data to be set
   */
  void set(const QVector<QPair<QString, QByteArray>> data);
};
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
