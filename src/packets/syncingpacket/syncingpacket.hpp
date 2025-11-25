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
#include "packets/network_packet.hpp"
#include "packets/packet_type.hpp"
#include "common/types/enums/enums.hpp"
#include "common/types/exceptions/exceptions.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
/**
 * @brief Clipboard Sync Packet's Payload
 */
class SyncingItem {
 private:

  QByteArray mimeType;
  QByteArray payload;

 public:

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
class SyncingPacket: public NetworkPacket {
 private:  // private members

  quint32 packetType = PacketType::SYNCING_PACKET;
  QVector<SyncingItem> items;

 public:

  /**
   * @brief Get the Packet Length object
   *
   * @return qint32
   */
  quint32 getPacketLength() const noexcept;

  /**
   * @brief Get the Packet Type object
   *
   * @return quint32
   */
  quint32 getPacketType() const noexcept;

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
   * @brief to Bytes
   */
  QByteArray toBytes() const override;

  /**
   * @brief From Bytes
   */
  static SyncingPacket fromBytes(const QByteArray &array);
};
}  // namespace srilakshmikanthanp::clipbirdesk::packets
