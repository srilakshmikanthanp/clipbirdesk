#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QtGlobal>

namespace srilakshmikanthanp::clipbirdesk::packets {
enum PacketType : quint32 {
  AUTHENTICATION_PACKET = 0x01,
  INVALID_REQUEST = 0x00,
  PING_PONG_PACKET = 0x03,
  SYNCING_PACKET = 0x02,
  CERTIFICATE_EXCHANGE = 0x04,
};
}
