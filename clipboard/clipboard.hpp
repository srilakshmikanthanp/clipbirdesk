// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QByteArray>
#include <QBuffer>
#include <QClipboard>
#include <QMimeData>
#include <QObject>
#include <QString>
#include <QImage>
#include <QVariant>

// C++ header
#include <algorithm>
#include <vector>
#include <utility>

// project header
#include "types/except/except.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
/**
 * @brief Class to manage clipboard such get,
 * set and notify the clipboard change
 */
class Clipboard : public QObject {
 private:  // members
  QClipboard* m_clipboard = nullptr;  // clipboard that is managed by this class

 public:   // typedefs
  using OnClipboardChange = void (*)(std::pair<QString, QByteArray>);

 private:  // member
  std::vector<OnClipboardChange> m_listeners;

 private:  // slots
  /**
   * @brief Slot to receive the clipboard change
   * and notify the listeners
   */
  void onChange() {
    std::pair<QString, QByteArray> clipboardData;

    try {
      clipboardData = this->get();
    } catch (const std::exception& e) {
      return;
    }

    for (const auto& listener : m_listeners) {
      listener(clipboardData);
    }
  }

 public:   // constructor
  /**
   * @brief Construct a new Clipboard object and manage
   * the clipboard that is passed via the constructor
   *
   * @param clipboard Clipboard that is managed
   * @param parent parent object
   */
  explicit Clipboard(QClipboard* clipboard, QObject* parent)
      : QObject(parent), m_clipboard(clipboard) {
    // connect the clipboard change signal to the slot
    // that is used to notify the listeners
    const auto signal = &QClipboard::changed;
    const auto slot = &Clipboard::onChange;
    QObject::connect(m_clipboard, signal, this, slot);
  }

  /**
   * @brief Add listener to receive the clipboard change
   *
   * @param listener listener to be added
   */
  void addListener(OnClipboardChange listener) {
    m_listeners.push_back(listener);
  }

  /**
   * @brief Remove listener from the list
   *
   * @param listener listener to be removed
   */
  void removeListener(OnClipboardChange listener) {
    auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (it != m_listeners.end()) {
      m_listeners.erase(it);
    }
  }

  /**
   * @brief Set the clipboard data to the clipboard
   *
   * @param mime mime type of the data
   * @param data data to be set
   */
  void set(const QString& mime, const QByteArray& data) {
    QMimeData* mimeData = new QMimeData();
    mimeData->setData(mime, data);
    m_clipboard->setMimeData(mimeData);
  }

  /**
   * @brief Clear the clipboard content
   */
  void clear() { m_clipboard->clear(); }

  /**
   * @brief Get the clipboard data from the clipboard
   *
   * @return mime type and data
   */
  std::pair<QString, QByteArray> get() {
    const auto mimeData = m_clipboard->mimeData();

    if (mimeData->hasImage()) {
      const auto image = qvariant_cast<QImage>(mimeData->imageData());
      auto buffer = QBuffer();
      buffer.open(QIODevice::WriteOnly);
      image.save(&buffer, "PNG");
      return std::make_pair("image/png", buffer.data());
    }

    if (mimeData->hasColor()) {
      const auto data = mimeData->data("application/x-color");
      return std::make_pair("application/x-color", data);
    }

    if (mimeData->hasText()) {
      const auto data = mimeData->data("text/plain");
      const auto text = QString::fromUtf8(data);
      return std::make_pair("text/plain", text.toUtf8());
    }

    if (mimeData->hasHtml()) {
      const auto data = mimeData->data("text/html");
      const auto text = QString::fromUtf8(data);
      return std::make_pair("text/html", text.toUtf8());
    }

    if (mimeData->hasUrls()) {
      const auto data = mimeData->data("text/uri-list");
      return std::make_pair("text/uri-list", data);
    }

    // If the data is not supported throw exception
    throw types::except::NotSupported("Not supported");
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
