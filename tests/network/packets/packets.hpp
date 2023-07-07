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
#include "../../../network/packets/packets.hpp"
#include "../../../types/types.hpp"
#include "../../../utility/functions/packet.hpp"

/**
 * @brief testing the ServiceDiscoveryPacket
 */
TEST(ServerDiscoveryPacketTest, TestingServerDiscoveryPacket) {
  //  using the sizeOfPacket
  using srilakshmikanthanp::clipbirdesk::utility::functions::packet::sizeOfPacket;

  // using the IPType
  typedef srilakshmikanthanp::clipbirdesk::types::IPType IPType;

  // using the ServiceDiscoveryPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::ServiceDiscoveryPacket;

  // constant ip
  const auto ip = QByteArray("\x7F\x00\x00\x01", 4);

  // constant port
  const auto port = 1234;

  // creating the packet
  ServiceDiscoveryPacket packet_send, packet_recv;

  // setting the packet type
  packet_send.setPacketType(ServiceDiscoveryPacket::PacketType::Request);

  // setting the ip type
  packet_send.setIpType(IPType::IPv4);

  // setting the ip value
  packet_send.setHostIp(ip);

  // setting the port
  packet_send.setHostPort(port);

  // setting the packet length
  packet_send.setPacketLength(sizeOfPacket(packet_send));

  // load the packet from network byte order
  packet_recv.fromNetBytes(packet_send.toNetBytes());

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), ServiceDiscoveryPacket::PacketType::Request);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), sizeOfPacket(packet_recv));

  // check the ip type
  EXPECT_EQ(packet_recv.getIpType(), IPType::IPv4);

  // check the ip value
  EXPECT_EQ(packet_recv.getHostIp(), ip);

  // check the port
  EXPECT_EQ(packet_recv.getHostPort(), port);
}

/**
 * @brief testing the ServiceDiscoveryPacket
 */
TEST(ClipbirdSyncPacket, TestingClipbirdSyncPacket) {
  // using the sizeOfPacket
  using srilakshmikanthanp::clipbirdesk::utility::functions::packet::sizeOfPacket;

  // using the ClipbirdSyncPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::ClipbirdSyncPacket;

  // data type
  const auto dataType = QByteArray("text/plain", 10);

  // data
  const auto data = QByteArray("Hello World", 11);

  // creating the packet
  ClipbirdSyncPacket packet_send, packet_recv;

  // setting the packet type
  packet_send.setPacketType(ClipbirdSyncPacket::PacketType::SyncPacket);

  // setting the data type length
  packet_send.setDataTypeLength(dataType.size());

  // setting the data type
  packet_send.setDataType(dataType);

  // setting the data length
  packet_send.setDataLength(data.size());

  // setting the data
  packet_send.setData(data);

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
  EXPECT_EQ(packet_recv.getDataType(), dataType);

  // check the data length
  EXPECT_EQ(packet_recv.getDataLength(), data.size());

  // check the data
  EXPECT_EQ(packet_recv.getData(), data);
}
