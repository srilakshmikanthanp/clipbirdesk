#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

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
#include <QList>
#include <QVector>

// project header
#include "types/except/except.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
/**
 * @brief Class to manage clipboard such get,
 * set and notify the clipboard change
 */
class Clipboard : public QObject {
 signals:   // signals
  void OnClipboardChange(QVector<QPair<QString, QByteArray>>);

 private:   // just for Qt
  /// Qt meta object
  Q_OBJECT

 private:   // members
  QClipboard* m_clipboard = nullptr;  // clipboard that is managed by this class

 private:   // slots
  /**
   * @brief Slot to receive the clipboard change
   * and notify the listeners
   */
  void onClipboardChange() {
    // default clipboard data
    QVector<QPair<QString, QByteArray>> clipboardData;

    // try to get the data
    try {
      clipboardData = this->get();
    } catch (const std::exception& e) {
      return;
    }

    // notify the listeners
    emit OnClipboardChange(clipboardData);
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
    const auto slot = &Clipboard::onClipboardChange;
    QObject::connect(m_clipboard, signal, this, slot);
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
      throw types::except::NotSupported("Error");
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
  void set(const QVector<QPair<QString, QByteArray>> data) {
    // create the mime data object
    auto mimeData = new QMimeData();

    // set the data
    for (const auto& [mime, data] : data) {
      mimeData->setData(mime, data);
    }

    // set the mime data
    m_clipboard->setMimeData(mimeData);
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard
