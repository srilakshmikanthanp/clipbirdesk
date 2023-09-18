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
#include "network/packets/authentication/authentication.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

/**
 * @brief testing the AuthenticationPacket
 */
TEST(AuthenticationTest, TestingAuthentication) {
  // using the AuthenticationPacket
  using srilakshmikanthanp::clipbirdesk::network::packets::Authentication;

  // using the ErrorCode
  using srilakshmikanthanp::clipbirdesk::types::enums::AuthStatus;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  Authentication packet_send, packet_recv;

  // constant values
  const auto packetType = Authentication::PacketType::AuthStatus;

  // setting the packet type
  packet_send = createPacket({ packetType, AuthStatus::AuthOkay });

  // load the packet from network byte order
  packet_recv = fromQByteArray<Authentication>(toQByteArray(packet_send));

  // check the packet type
  EXPECT_EQ(packet_recv.getPacketType(), packetType);

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.size());

  // check the status code
  EXPECT_EQ(packet_recv.getAuthStatus(), AuthStatus::AuthOkay);
}
