#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Standard header files
#include <iostream>
#include <stdexcept>

// Qt header files
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QtTypes>

// Local header files
#include "types/enums/enums.hpp"
#include "types/except/except.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Clipboard Sync Packet's Payload
 */
class SyncingItem {
 private:

  quint32 mimeLength;
  QByteArray mimeType;
  quint32 payloadLength;
  QByteArray payload;

 public:

  /**
   * @brief Set the Mime Length object
   */
  void setMimeLength(quint32 length);

  /**
   * @brief Get the Mime Length object
   *
   * @return qint32
   */
  quint32 getMimeLength() const noexcept;

  /**
   * @brief Set the Mime Type object
   *
   * @param type
   */
  void setMimeType(const QByteArray& type);

  /**
   * @brief Get the Mime Type object
   *
   * @return QByteArray
   */
  QByteArray getMimeType() const noexcept;

  /**
   * @brief Set the Payload Length object
   *
   * @param length
   */
  void setPayloadLength(quint32 length);

  /**
   * @brief Get the Payload Length object
   *
   * @return qint32
   */
  quint32 getPayloadLength() const noexcept;

  /**
   * @brief Set the Payload object
   *
   * @param payload
   */
  void setPayload(const QByteArray& payload);

  /**
   * @brief Get the Payload object
   *
   * @return QByteArray
   */
  QByteArray getPayload() const noexcept;

  /**
   * @brief Get the size of the packet
   *
   * @return size_t
   */
  quint32 size() const noexcept;

  /**
   * @brief Overloaded operator<< for QDataStream
   *
   * @param out
   * @param payload
   */
  friend QDataStream& operator<<(QDataStream& out, const SyncingItem& payload);

  /**
   * @brief Overloaded operator>> for QDataStream
   *
   * @param in
   * @param payload
   */
  friend QDataStream& operator>>(QDataStream& in, SyncingItem& payload);
};
}
