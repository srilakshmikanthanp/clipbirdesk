#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Google test header files
#include <gtest/gtest.h>

// Qt header files
#include <QByteArray>

// Local header files
#include "packets/syncingpacket/syncingpacket.hpp"
#include "common/types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

/**
 * @brief testing the ServiceDiscoveryPacket
 */
TEST(SyncingPacket, TestingSyncingPacket) {
  // using the ClipboardSyncPacket
  using srilakshmikanthanp::clipbirdesk::packets::SyncingItem;
  using srilakshmikanthanp::clipbirdesk::packets::SyncingPacket;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  SyncingPacket packet_send, packet_recv;

  // constant values
  const auto itemCount  = 2;
  const auto mimeType   = QByteArray("text/plain", 10);
  const auto payload    = QByteArray("Hello World", 11);

  // create items
  QVector<QPair<QString, QByteArray>> items;

  // creating the items
  for (auto i = 0; i < itemCount; i++) {
    items.push_back({mimeType, payload});
  }

  // setting the packet type
  packet_send = createPacket({items});

  // load the packet from network byte order
  packet_recv = fromQByteArray<SyncingPacket>(toQByteArray(packet_send));

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.getPacketLength());

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
