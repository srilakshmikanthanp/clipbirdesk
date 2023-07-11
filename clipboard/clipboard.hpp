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
  /**
   * @brief Signal to notify the clipboard change occurrence use
   * the parameter or get method to get the clipboard data
   * @param items clipboard data
   */
  void OnClipboardChange(QVector<QPair<QString, QByteArray>>);

 private:   // members
  QClipboard* m_clipboard = nullptr;

 private:   // just for Qt
  /// @brief Qt meta object
  Q_OBJECT

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
    const auto func = &Clipboard::OnClipboardChange;
    const auto signal = &QClipboard::changed;
    const auto slot = [this, func]{ emit (this->*func)(get()); };
    QObject::connect(m_clipboard, signal, this, slot);
  }

  /**
   * @brief Get the clipboard data from the clipboard
   *
   * @return mime type and data
   */
  QVector<QPair<QString, QByteArray>> get() {
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
