#include "applicationclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
/**
 * @brief Slot to notify the clipboard change
 */
void ApplicationClipboard::onClipboardChangeImpl(QClipboard::Mode mode) {
  if (!QApplication::clipboard()->ownsClipboard() && mode == QClipboard::Mode::Clipboard) {
    this->get().then([this](QVector<QPair<QString, QByteArray>> result){ if (!result.isEmpty()) emit OnClipboardChange(result); });
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
QFuture<QVector<QPair<QString, QByteArray>>> ApplicationClipboard::get() const {
  const auto mimeData = m_clipboard->mimeData(QClipboard::Mode::Clipboard);
  QVector<QPair<QString, QByteArray>> items;
  std::optional<QImage> image;

  if (mimeData->hasHtml()) {
    items.append({MIME_TYPE_HTML, mimeData->html().toUtf8()});
  }

  if (mimeData->hasText()) {
    items.append({MIME_TYPE_TEXT, mimeData->text().toUtf8()});
  }

  if (mimeData->hasImage()) {
    image = qvariant_cast<QImage>(mimeData->imageData());
  }

  return QtConcurrent::run([items, image, this]() mutable {
    if (image.has_value() && !image->isNull()) {
      QByteArray byteArray;
      QBuffer buffer(&byteArray);
      buffer.open(QIODevice::WriteOnly);
      image->save(&buffer, IMAGE_TYPE_PNG);
      items.append({MIME_TYPE_PNG, byteArray});
    }
    return items;
  });
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
