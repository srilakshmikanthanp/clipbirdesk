#pragma once

#include <QString>
#include <QHostAddress>

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
struct NetResolvedDevice {
  QString name;
  QHostAddress host;
  quint16 port;

  bool operator==(const NetResolvedDevice& other) const {
    return name == other.name;
  }

  bool operator!=(const NetResolvedDevice& other) const {
    return !(*this == other);
  }

  bool operator<(const NetResolvedDevice& other) const {
    return name < other.name;
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network

template <>
struct std::hash<srilakshmikanthanp::clipbirdesk::syncing::network::NetResolvedDevice> {
  std::size_t operator()(
    const srilakshmikanthanp::clipbirdesk::syncing::network::NetResolvedDevice& device
  ) const noexcept {
    return std::hash<QString>{}(device.name);
  }
};
