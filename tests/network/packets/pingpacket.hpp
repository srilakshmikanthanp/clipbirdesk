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
#include "network/packets/pingpacket/pingpacket.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

/**
 * @brief testing the PingPacket
 */
TEST(PingPacket, TestingPingPacket) {
  // using the AuthenticationPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::PingPacket;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  PingPacket packet_send, packet_recv;

  // constant values
  const auto packetType = PingPacket::PacketType::PingPong;
  const auto pingType  = PingPacket::PingType::Ping;

  // create packet
  packet_send = createPacket(params::PingPacketParams{packetType, pingType});

  // to network byte order
  packet_recv = fromQByteArray<PingPacket>(toQByteArray(packet_send));

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

  // check the ping type
  EXPECT_EQ(packet_recv.getPingType(), pingType);

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), packetType);
}
