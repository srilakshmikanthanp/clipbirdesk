#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <gtest/gtest.h>

#include "../../../network/packets/packets.hpp"
#include "../../../utility/functions/packet.hpp"
#include "../../../utility/typeconv/veries.hpp"

/**
 * @brief testing the ServerDiscoveryPacket
 */
TEST(ServerDiscoveryPacketTest, TestingServerDiscoveryPacket) {
  //  using the sizeOfPacket
  using srilakshmikanthanp::clipbirdesk::utility::functions::packet::sizeOfPacket;

  // using the ServerDiscoveryPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::ServerDiscoveryPacket;

  // constant ip
  const auto ip = std::vector<uint8_t>({127, 127, 127, 127});

  // constant port
  const auto port = 1234;

  // creating the packet
  ServerDiscoveryPacket packet_send, packet_recv;

  // setting the packet type
  packet_send.setPacketType(ServerDiscoveryPacket::PacketType::Request);

  // setting the ip type
  packet_send.setIpType(ServerDiscoveryPacket::IpType::Ipv4);

  // setting the ip value
  packet_send.setHostIp(ip);

  // setting the port
  packet_send.setHostPort(port);

  // setting the packet length
  packet_send.setPacketLength(sizeOfPacket(packet_send));

  // load the packet from network byte order
  packet_recv.fromNetBytes(packet_send.toNetBytes());

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), ServerDiscoveryPacket::PacketType::Request);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), sizeOfPacket(packet_recv));

  // check the ip type
  EXPECT_EQ(packet_recv.getIpType(), ServerDiscoveryPacket::IpType::Ipv4);

  // check the ip value
  EXPECT_EQ(packet_recv.getHostIp(), ip);

  // check the port
  EXPECT_EQ(packet_recv.getHostPort(), port);
}

/**
 * @brief testing the ServerDiscoveryPacket
 */
TEST(ClipbirdSyncPacket, TestingClipbirdSyncPacket) {
  // using the sizeOfPacket
  using srilakshmikanthanp::clipbirdesk::utility::functions::packet::sizeOfPacket;

  // using the ClipbirdSyncPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::ClipbirdSyncPacket;

  // using to varies
  using srilakshmikanthanp::clipbirdesk::utility::typeconv::varies::to_varies;

  // data type
  const auto dataType = std::string("text/plain");

  // data
  const auto data = std::string("Hello World");

  // creating the packet
  ClipbirdSyncPacket packet_send, packet_recv;

  // setting the packet type
  packet_send.setPacketType(ClipbirdSyncPacket::PacketType::SyncPacket);

  // setting the data type length
  packet_send.setDataTypeLength(dataType.size());

  // setting the data type
  packet_send.setDataType(to_varies(dataType));

  // setting the data length
  packet_send.setDataLength(data.size());

  // setting the data
  packet_send.setData(to_varies(data));

  // setting the packet length
  packet_send.setPacketLength(sizeOfPacket(packet_send));

  // load the packet from network byte order
  packet_recv.fromNetBytes(packet_send.toNetBytes());

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), ClipbirdSyncPacket::PacketType::SyncPacket);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), sizeOfPacket(packet_recv));

  // check the data type length
  EXPECT_EQ(packet_recv.getDataTypeLength(), dataType.size());

  // check the data type
  EXPECT_EQ(packet_recv.getDataType(), to_varies(dataType));

  // check the data length
  EXPECT_EQ(packet_recv.getDataLength(), data.size());

  // check the data
  EXPECT_EQ(packet_recv.getData(), to_varies(data));
}
