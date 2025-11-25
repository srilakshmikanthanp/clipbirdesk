#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt header
#include <QApplication>
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
#include <QtConcurrent>

// project header
#include "clipboard/platformclipboard.hpp"
#include "common/types/exceptions/exceptions.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {

/**
 * @brief Class to manage clipboard such get,
 * set and notify the clipboard change
 */
class ApplicationClipboard : public QObject {
 signals:  // signals
  /**
   * @brief Signal to notify the clipboard change occurrence use
   * the parameter or get method to get the clipboard data
   * @param items clipboard data
   */
  void OnClipboardChange(QVector<QPair<QString, QByteArray>>);

 private:  // members

  PlatformClipboard *m_clipboard = PlatformClipboard::instance();

 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(ApplicationClipboard)

 private:  // private slots

  /// @brief Slot to notify the clipboard change
  void onClipboardChangeImpl(QClipboard::Mode mode);

 private:  // mime types

  const QString MIME_TYPE_TEXT  = "text/plain";
  const QString MIME_TYPE_PNG   = "image/png";
  const QString MIME_TYPE_HTML  = "text/html";

 private: // image type

  const char* IMAGE_TYPE_PNG = "PNG";

 public:  // constructor

  /**
   * @brief Construct a new Clipboard object and manage
   * the clipboard that is passed via the constructor
   *
   * @param clipboard Clipboard that is managed
   * @param parent parent object
   */
  explicit ApplicationClipboard(QObject* parent = nullptr);

  /**
   * @brief Get the clipboard data from the clipboard
   *
   * @return mime type and data
   */
  QFuture<QVector<QPair<QString, QByteArray>>> get() const;

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
