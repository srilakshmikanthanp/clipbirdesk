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
#include "network/packets/DiscoveryPacket.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/nbytes.hpp"

/**
 * @brief testing the ServiceDiscoveryPacket
 */
TEST(DiscoveryPacketTest, TestingDiscoveryPacket) {
  // using the ServiceDiscoveryPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::DiscoveryPacket;

  // using the IPType
  typedef srilakshmikanthanp::clipbirdesk::types::enums::IPType IPType;;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  DiscoveryPacket packet_send, packet_recv;

  // Constant values
  const auto packetType = DiscoveryPacket::PacketType::Request;
  const auto ipType = IPType::IPv4;
  const auto ip = QByteArray("\x7F\x00\x00\x01", 4);
  const auto port = 1234;

  // setting the packet type
  packet_send.setPacketType(packetType);

  // setting the ip type
  packet_send.setIpType(ipType);

  // setting the ip value
  packet_send.setHostIp(ip);

  // setting the port
  packet_send.setHostPort(port);

  // setting the packet length
  packet_send.setPacketLength(packet_send.size());

  // load the packet from network byte order
  packet_recv = fromQByteArray<DiscoveryPacket>(toQByteArray(packet_send));

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), packetType);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

  // check the ip type
  EXPECT_EQ(packet_recv.getIpType(), ipType);

  // check the ip value
  EXPECT_EQ(packet_recv.getHostIp(), ip);

  // check the port
  EXPECT_EQ(packet_recv.getHostPort(), port);
}
