#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Google test header files
#include <gtest/gtest.h>

// Qt header files
#include <QByteArray>

// Local header files
#include "packets/authentication/authentication.hpp"
#include "common/types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

/**
 * @brief testing the AuthenticationPacket
 */
TEST(AuthenticationTest, TestingAuthentication) {
  // using the AuthenticationPacket
  using srilakshmikanthanp::clipbirdesk::packets::Authentication;

  // using the ErrorCode
  using srilakshmikanthanp::clipbirdesk::common::types::enums::AuthStatus;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  Authentication packet_send, packet_recv;

  // setting the packet type
  packet_send = createPacket(params::AuthenticationParams{ AuthStatus::AuthOkay });

  // load the packet from network byte order
  packet_recv = fromQByteArray<Authentication>(toQByteArray(packet_send));

  // check the packet length
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.getPacketLength());

  // check the status code
  EXPECT_EQ(packet_recv.getAuthStatus(), AuthStatus::AuthOkay);
}
