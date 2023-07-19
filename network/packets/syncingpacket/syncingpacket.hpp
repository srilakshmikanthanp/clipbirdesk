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
#include "network/packets/syncingitem/syncingitem.hpp"
#include "types/enums/enums.hpp"
#include "types/except/except.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Clipboard Sync Packet
 */
class SyncingPacket {
 private:  // private members

  quint8 packetType = 0x03;
  qint32 packetLength;
  qint32 itemCount;
  QVector<SyncingItem> items;

 public:

  /// @brief Allowed Packet Types
  enum PacketType : quint8 { SyncPacket = 0x03 };

 public:

  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint8 type);

  /**
   * @brief Get the Packet Type object
   *
   * @return quint8
   */
  quint8 getPacketType() const noexcept;

  /**
   * @brief Set the Packet Length object
   *
   * @param length
   */
  void setPacketLength(qint32 length);

  /**
   * @brief Get the Packet Length object
   *
   * @return qint32
   */
  qint32 getPacketLength() const noexcept;

  /**
   * @brief Set the Item Count object
   *
   * @param count
   */
  void setItemCount(qint32 count);

  /**
   * @brief Get the Item Count object
   *
   * @return qint32
   */
  qint32 getItemCount() const noexcept;

  /**
   * @brief Set the Payloads object
   *
   * @param payloads
   */
  void setItems(const QVector<SyncingItem>& payloads);

  /**
   * @brief Get the Payloads object
   *
   * @return QVector<Payload>
   */
  QVector<SyncingItem> getItems() const noexcept;

  /**
   * @brief Get the size of the packet
   *
   * @return size_t
   */
  size_t size() const noexcept;

  /**
   * @brief Overloaded operator<< for QDataStream
   *
   * @param out
   * @param packet
   */
  friend QDataStream& operator<<(QDataStream& out, const SyncingPacket& packet);

  /**
   * @brief Overloaded operator>> for QDataStream
   *
   * @param in
   * @param packet
   */
  friend QDataStream& operator>>(QDataStream& in, SyncingPacket& packet);
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
