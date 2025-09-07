#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Google test header files
#include <gtest/gtest.h>

// Qt header files
#include <QByteArray>

// Local header files
#include "packets/pingpongpacket/pingpongpacket.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

/**
 * @brief testing the PingPongPacket
 */
TEST(PingPongPacket, TestingPingPacket) {
  // using the AuthenticationPacket
  using srilakshmikanthanp::clipbirdesk::packets::PingPongPacket;

    // using the ErrorCode
  using srilakshmikanthanp::clipbirdesk::types::enums::PingType;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  PingPongPacket packet_send, packet_recv;

  // constant values
  const auto packetType = PingPongPacket::PacketType::PingPong;
  const auto pingType  = PingType::Ping;

  // create packet
  packet_send = createPacket(params::PingPacketParams{packetType, pingType});

  // to network byte order
  packet_recv = fromQByteArray<PingPongPacket>(toQByteArray(packet_send));

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

  // check the ping type
  EXPECT_EQ(packet_recv.getPingType(), pingType);

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), packetType);
}
