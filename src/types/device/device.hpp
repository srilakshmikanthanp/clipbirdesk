#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QHostAddress>
#include <QSslCertificate>

namespace srilakshmikanthanp::clipbirdesk::types::device {
/**
 * @brief Device Class That Represents Device
 */
struct Device {
  // operator overloading for comparison
  bool operator==(const Device& other) const;
  bool operator!=(const Device& other) const;

  // public members
  QHostAddress ip;
  quint16 port;
  QString name;
};
}  // namespace srilakshmikanthanp::types::device
