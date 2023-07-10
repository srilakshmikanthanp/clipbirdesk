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
#include "network/packets/packets.hpp"
#include "types/enums/enums.hpp"

/**
 * @brief testing the MalformedPacket
 */
TEST(MalformedPacketTest, TestingMalformedPacket) {
  // using the MalformedPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::MalformedPacket;

  // creating the packet
  MalformedPacket packet_send, packet_recv;

  // setting the packet type
  packet_send.setPacketType(MalformedPacket::PacketType::InvalidPacket);

  // setting the packet length
  packet_send.setErrorCode(MalformedPacket::ErrorCode::CodingError);

  // setting the message
  packet_send.setErrorMessage("Hello");

  // setting the packet length
  packet_send.setPacketLength(packet_send.size());

  // load the packet from network byte order
  packet_recv.fromNetBytes(packet_send.toNetBytes());

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), MalformedPacket::PacketType::InvalidPacket);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_recv.size());

  // check the error code
  EXPECT_EQ(packet_recv.getErrorCode(), MalformedPacket::ErrorCode::CodingError);

  // check the message
  EXPECT_EQ(packet_recv.getErrorMessage(), "Hello");
}

/**
 * @brief testing the ServiceDiscoveryPacket
 */
TEST(ServerDiscoveryPacketTest, TestingServerDiscoveryPacket) {
  // using the ServiceDiscoveryPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::ServiceDiscoveryPacket;

  // using the IPType
  typedef srilakshmikanthanp::clipbirdesk::types::enums::IPType IPType;

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
  packet_send.setPacketLength(packet_send.size());

  // load the packet from network byte order
  packet_recv.fromNetBytes(packet_send.toNetBytes());

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), ServiceDiscoveryPacket::PacketType::Request);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

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
TEST(ClipboardSyncPacket, TestingClipbirdSyncPacket) {
  // using the ClipboardSyncPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::ClipboardSyncPacket;

  // data type
  const auto mimeType = QByteArray("text/plain", 10);

  // data
  const auto data = QByteArray("Hello World", 11);

  // creating the packet
  ClipboardSyncPacket packet_send, packet_recv;

  // setting the packet type
  packet_send.setPacketType(ClipboardSyncPacket::PacketType::SyncPacket);

  // setting the data type length
  packet_send.setMimeLength(mimeType.size());

  // setting the data type
  packet_send.setMimeType(mimeType);

  // setting the data length
  packet_send.setPayloadLength(data.size());

  // setting the data
  packet_send.setPayload(data);

  // setting the packet length
  packet_send.setPacketLength(packet_send.size());

  // load the packet from network byte order
  packet_recv.fromNetBytes(packet_send.toNetBytes());

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), ClipboardSyncPacket::PacketType::SyncPacket);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

  // check the data type length
  EXPECT_EQ(packet_recv.getMimeLength(), mimeType.size());

  // check the data type
  EXPECT_EQ(packet_recv.getMimeType(), mimeType);

  // check the data length
  EXPECT_EQ(packet_recv.getPayloadLength(), data.size());

  // check the data
  EXPECT_EQ(packet_recv.getPayload(), data);
}
