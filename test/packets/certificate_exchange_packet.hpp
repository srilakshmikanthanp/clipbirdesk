#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Google test header files
#include <gtest/gtest.h>

// Qt header files
#include <QByteArray>

// Local header files
#include "packets/certificate_exchange_packet/certificate_exchange_packet.hpp"
#include "common/types/enums/enums.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

/**
 * @brief testing the CertificateExchangePacket
 */
TEST(CertificateExchangePacket, TestingCertificateExchangePacket) {
  // using the CertificateExchangePacket
  using srilakshmikanthanp::clipbirdesk::packets::CertificateExchangePacket;
  using srilakshmikanthanp::clipbirdesk::packets::PacketType;

  // using functions namespace
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

  // creating the packet
  CertificateExchangePacket packet_send, packet_recv;

  // constant values
  const auto certificateBytes = QByteArray("DummyCertificateData", 20);

  // setting the certificate
  packet_send.setCertificate(certificateBytes);

  // load the packet from network byte order
  packet_recv = fromQByteArray<CertificateExchangePacket>(toQByteArray(packet_send));

  EXPECT_EQ(packet_recv.getPacketType(), packet_send.getPacketType());
  EXPECT_EQ(packet_recv.getPacketLength(), packet_send.getPacketLength());
  EXPECT_EQ(packet_recv.getCertificateSize(), certificateBytes.size());
  EXPECT_EQ(packet_recv.getCertificate(), certificateBytes);
}
