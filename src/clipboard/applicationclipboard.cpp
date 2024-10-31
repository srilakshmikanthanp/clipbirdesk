/**
 * Copyright (c) 2023 Sri Lakshmi Kanthan P
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "applicationclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
/**
 * @brief Slot to notify the clipboard change
 */
void ApplicationClipboard::onClipboardChangeImpl(QClipboard::Mode mode) {
  if (mode != QClipboard::Mode::Clipboard) return;
  if (!QApplication::clipboard()->ownsClipboard()) {
    Q_UNUSED(QtConcurrent::run([this]() { emit OnClipboardChange(this->get()); }));
  }
}

/**
 * @brief Construct a new Clipboard object and manage
 * the clipboard that is passed via the constructor
 *
 * @param clipboard Clipboard that is managed
 * @param parent parent object
 */
ApplicationClipboard::ApplicationClipboard(QObject* parent) : QObject(parent) {
  // connect the clipboard change signal to the slot
  QObject::connect(
    this->m_clipboard, &PlatformClipboard::changed,
    this, &ApplicationClipboard::onClipboardChangeImpl
  );
}

/**
 * @brief Get the clipboard data from the clipboard
 *
 * @return mime type and data
 */
QVector<QPair<QString, QByteArray>> ApplicationClipboard::get() const {
  // Default clipboard data & mime data
  QVector<QPair<QString, QByteArray>> items;

  // get the mime data
  const auto mimeData = m_clipboard->mimeData(QClipboard::Mode::Clipboard);

  // if mime data is not supported
  if (mimeData == nullptr) return items;

  // has HTML
  if (mimeData->hasHtml()) {
    items.append({MIME_TYPE_HTML, mimeData->html().toUtf8()});
  }

  // has Image
  if (mimeData->hasImage()) {
    auto image = qvariant_cast<QImage>(mimeData->imageData());
    QByteArray data; QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, IMAGE_TYPE_PNG);
    items.append({MIME_TYPE_PNG, data});
  }

  // has Text
  if (mimeData->hasText()) {
    items.append({MIME_TYPE_TEXT, mimeData->text().toUtf8()});
  }

  // return the data
  return items;
}

/**
 * @brief Clear the clipboard content
 */
void ApplicationClipboard::clear() {
  m_clipboard->clear(QClipboard::Mode::Clipboard);
}

/**
 * @brief Set the clipboard data to the clipboard
 *
 * @param mime mime type of the data
 * @param data data to be set
 */
void ApplicationClipboard::set(const QVector<QPair<QString, QByteArray>> data) {
  // create the mime data object
  QMimeData *mimeData = new QMimeData();

  // set the data
  for (const auto& [mime, data] : data) {
    // has Image png
    if (mime == MIME_TYPE_PNG) {
      mimeData->setImageData(QImage::fromData(data, IMAGE_TYPE_PNG));
    }

    // has HTML
    if (mime == MIME_TYPE_HTML) {
      mimeData->setHtml(QString::fromUtf8(data));
    }

    // has Text
    if (mime == MIME_TYPE_TEXT) {
      mimeData->setText(QString::fromUtf8(data));
    }
  }

  // set the mime data
  m_clipboard->setMimeData(mimeData, QClipboard::Mode::Clipboard);
}
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
