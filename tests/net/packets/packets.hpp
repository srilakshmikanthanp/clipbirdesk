#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <gtest/gtest.h>

#include "../../../net/packets/packets.hpp"
#include "../../../utility/functions/packet.hpp"

TEST(ServerDiscoveryPacketTest, TestingServerDiscoveryPacket) {
  //  using the sizeOfPacket
  using srilakshmikanthanp::clipbirdesk::utility::functions::packet::sizeOfPacket;

  // using the ServerDiscoveryPacket
  using srilakshmikanthanp::clipbirdesk::net::packets::ServerDiscoveryPacket;

  // creating the packet
  ServerDiscoveryPacket packet_send;

  // setting the packet type
  packet_send.setPacketType(ServerDiscoveryPacket::PacketType::Request);

  // setting the ip type
  packet_send.setIpType(ServerDiscoveryPacket::IpType::Ipv4);

  // setting the ip value
  packet_send.setHostIp({127, 127, 127, 127});

  // setting the port
  packet_send.setHostPort(1234);

  // setting the packet length
  packet_send.setPacketLength(sizeOfPacket(packet_send));

  // create a new packet
  ServerDiscoveryPacket packet_recv;

  // load the packet from network byte order
  packet_recv.fromNetBytes(packet_send.toNetBytes());

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), ServerDiscoveryPacket::PacketType::Request);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), sizeOfPacket(packet_recv));

  // check the ip type
  EXPECT_EQ(packet_recv.getIpType(), ServerDiscoveryPacket::IpType::Ipv4);

  // check the ip value
  EXPECT_EQ(packet_recv.getHostIp(), std::vector<uint8_t>({127, 127, 127, 127}));

  // check the port
  EXPECT_EQ(packet_recv.getHostPort(), 1234);
}
