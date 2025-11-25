#pragma once

#include <QString>
#include <QBluetoothAddress>
#include <QBluetoothUuid>

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
struct BtResolvedDevice {
  QString name;
  QBluetoothAddress address;
  QBluetoothUuid uuid;

  bool operator==(const BtResolvedDevice& other) const {
    return name == other.name;
  }

  bool operator!=(const BtResolvedDevice& other) const {
    return !(*this == other);
  }

  bool operator<(const BtResolvedDevice& other) const {
    return name < other.name;
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth

template <>
struct std::hash<srilakshmikanthanp::clipbirdesk::syncing::bluetooth::BtResolvedDevice> {
  std::size_t operator()(
    const srilakshmikanthanp::clipbirdesk::syncing::bluetooth::BtResolvedDevice& device
  ) const noexcept {
    return std::hash<QString>{}(device.name);
  }
};
