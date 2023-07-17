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
#include "network/packets//invalidrequest/invalidrequest.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"

/**
 * @brief testing the MalformedPacket
 */
TEST(InvalidPacketTest, TestingInvalidPacket) {
  // using the MalformedPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::InvalidRequest;

  // using the ErrorCode
  using srilakshmikanthanp::clipbirdesk::types::enums::ErrorCode;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  InvalidRequest packet_send, packet_recv;

  // constant values
  const auto packetType = InvalidRequest::PacketType::RequestFailed;
  const auto errorCode = ErrorCode::CodingError;
  const auto errorMessage = QByteArray("Hello", 5);

  // setting the packet type
  packet_send.setPacketType(packetType);

  // setting the packet length
  packet_send.setErrorCode(errorCode);

  // setting the message
  packet_send.setErrorMessage(errorMessage);

  // setting the packet length
  packet_send.setPacketLength(packet_send.size());

  // load the packet from network byte order
  packet_recv = fromQByteArray<InvalidRequest>(toQByteArray(packet_send));

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), packetType);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

  // check the error code
  EXPECT_EQ(packet_recv.getErrorCode(), errorCode);

  // check the message
  EXPECT_EQ(packet_recv.getErrorMessage(), errorMessage);
}

