#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QByteArray>

namespace srilakshmikanthanp::clipbirdesk::packets {
class NetworkPacket {
 public:
  explicit NetworkPacket() {}
  virtual ~NetworkPacket() = default;

  virtual QByteArray toBytes() const = 0;
};
}
