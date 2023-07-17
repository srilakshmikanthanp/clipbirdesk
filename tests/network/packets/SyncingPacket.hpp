#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard
// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Google test header files
#include <gtest/gtest.h>

// Qt header files
#include <QByteArray>

// Local header files
#include "network/packets/syncingpacket/syncingpacket.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"

/**
 * @brief testing the ServiceDiscoveryPacket
 */
TEST(SyncingPacket, TestingSyncingPacket) {
  // using the ClipboardSyncPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::SyncingItem;
  using srilakshmikanthanp::clipbirdesk::network::packets::SyncingPacket;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  SyncingPacket packet_send, packet_recv;

  // constant values
  const auto packetType = SyncingPacket::PacketType::SyncPacket;
  const auto itemCount  = 2;
  const auto mimeType   = QByteArray("text/plain", 10);
  const auto payload    = QByteArray("Hello World", 11);
  QVector<SyncingItem> items;

  // creating the items
  for (auto i = 0; i < itemCount; i++) {
    // creating the payload
    SyncingItem item;

    // setting the mime length
    item.setMimeLength(mimeType.size());

    // setting the mime type
    item.setMimeType(mimeType);

    // setting the payload length
    item.setPayloadLength(payload.size());

    // setting the payload
    item.setPayload(payload);

    // adding the payload
    items.push_back(item);
  }

  // setting the packet type
  packet_send.setPacketType(packetType);

  // setting the item count
  packet_send.setItemCount(itemCount);

  // setting the items
  packet_send.setItems(items);

  // setting the packet length
  packet_send.setPacketLength(packet_send.size());

  // load the packet from network byte order
  packet_recv = fromQByteArray<SyncingPacket>(toQByteArray(packet_send));

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), packetType);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

  // check the item count
  EXPECT_EQ(packet_recv.getItemCount(), itemCount);

  // check the items
  for (const auto &item : packet_recv.getItems()) {
    // check the mime length
    EXPECT_EQ(item.getMimeLength(), mimeType.size());

    // check the mime type
    EXPECT_EQ(item.getMimeType(), mimeType);

    // check the payload length
    EXPECT_EQ(item.getPayloadLength(), payload.size());

    // check the payload
    EXPECT_EQ(item.getPayload(), payload);
  }
}
