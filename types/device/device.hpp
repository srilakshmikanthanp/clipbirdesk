#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

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
  QSslCertificate cert;
};
}  // namespace srilakshmikanthanp::types::device
