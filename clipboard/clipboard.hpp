// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QByteArray>
#include <QClipboard>
#include <QMimeData>
#include <QObject>
#include <QString>
#include <QPair>
#include <QVector>

// C++ header
#include <algorithm>

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
  using OnClipboardChange = void (*)(QVector<QPair<QString, QByteArray>>);

 private:  // member
  std::vector<OnClipboardChange> m_listeners;

 private:  // slots
  /**
   * @brief Slot to receive the clipboard change
   * and notify the listeners
   */
  void onChange() {
    // default clipboard data
    QVector<QPair<QString, QByteArray>> clipboardData;

    // try to get the data
    try {
      clipboardData = this->get();
    } catch (const std::exception& e) {
      return;
    }

    // notify the listeners
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
   * @brief Get the clipboard data from the clipboard
   *
   * @return mime type and data
   */
  QVector<QPair<QString, QByteArray>> get() {
    // Default clipboard data & mime data
    QVector<QPair<QString, QByteArray>> data;
    const auto mimeData = m_clipboard->mimeData();

    // if mime data is not supported
    if (mimeData == nullptr) {
      throw types::except::NotSupported("Clipboard data is not supported");
    }

    // get the formats
    const auto formats = mimeData->formats();

    // push the data to the vector
    for (const auto& format : formats) {
      data.push_back({format, mimeData->data(format)});
    }

    // return the data
    return data;
  }

  /**
   * @brief Clear the clipboard content
   */
  void clear() { m_clipboard->clear(); }

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
};
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
