/**
 * Copyright (c) 2023 Sri Lakshmi Kanthan P
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "clipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
/**
 * @brief Slot to notify the clipboard change
 */
void Clipboard::onClipboardChangeImpl() {
  if (!this->m_clipboard->ownsClipboard()) {
    emit OnClipboardChange(this->get());
  }
}

/**
 * @brief Construct a new Clipboard object and manage
 * the clipboard that is passed via the constructor
 *
 * @param clipboard Clipboard that is managed
 * @param parent parent object
 */
Clipboard::Clipboard(QClipboard* clipboard, QObject* parent)
    : QObject(parent), m_clipboard(clipboard) {
  // connect the clipboard change signal to the slot
  // that is used to notify the listeners
  const auto signal = &QClipboard::changed;
  const auto slot   = &Clipboard::onClipboardChangeImpl;
  QObject::connect(m_clipboard, signal, this, slot);
}

/**
 * @brief Get the clipboard data from the clipboard
 *
 * @return mime type and data
 */
QVector<QPair<QString, QByteArray>> Clipboard::get() {
  // Default clipboard data & mime data
  QVector<QPair<QString, QByteArray>> items;
  const auto mimeData = m_clipboard->mimeData();

  // if mime data is not supported
  if (mimeData == nullptr) return items;

  // get the formats
  const auto formats = mimeData->formats();

  // push the data to the vector
  for (const auto& format : formats) {
    items.push_back({format, mimeData->data(format)});
  }

  // return the data
  return items;
}

/**
 * @brief Clear the clipboard content
 */
void Clipboard::clear() {
  m_clipboard->clear();
}

/**
 * @brief Set the clipboard data to the clipboard
 *
 * @param mime mime type of the data
 * @param data data to be set
 */
void Clipboard::set(const QVector<QPair<QString, QByteArray>> data) {
  // create the mime data object
  auto mimeData = new QMimeData();

  // set the data
  for (const auto& [mime, data] : data) {
    mimeData->setData(mime, data);
  }

  // set the mime data
  m_clipboard->setMimeData(mimeData);
}
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
