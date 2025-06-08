#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

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
   * @brief To Stream
   */
  void toStream(QDataStream& stream) const;

  /**
   * @brief to Bytes
   */
  QByteArray toBytes() const;

  /**
   * @brief From Stream
   */
  static SyncingItem fromStream(QDataStream& stream);

  /**
   * @brief From Bytes
   */
  static SyncingItem fromBytes(const QByteArray &array);
};

/**
 * @brief Clipboard Sync Packet
 */
class SyncingPacket {
 public:

  /// @brief Allowed Packet Types
  enum PacketType : quint32 { SyncPacket = 0x02 };

 private:  // private members

  quint32 packetLength;
  quint32 packetType = PacketType::SyncPacket;
  quint32 itemCount;
  QVector<SyncingItem> items;

 public:

  /**
   * @brief Set the Packet Length object
   *
   * @param length
   */
  void setPacketLength(quint32 length);

  /**
   * @brief Get the Packet Length object
   *
   * @return qint32
   */
  quint32 getPacketLength() const noexcept;

  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint32 type);

  /**
   * @brief Get the Packet Type object
   *
   * @return quint32
   */
  quint32 getPacketType() const noexcept;

  /**
   * @brief Set the Item Count object
   *
   * @param count
   */
  void setItemCount(quint32 count);

  /**
   * @brief Get the Item Count object
   *
   * @return qint32
   */
  quint32 getItemCount() const noexcept;

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
  quint32 size() const noexcept;

  /**
   * @brief to Bytes
   */
  QByteArray toBytes() const;

  /**
   * @brief From Bytes
   */
  static SyncingPacket fromBytes(const QByteArray &array);
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
