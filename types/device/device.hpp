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
  QHostAddress ip;
  quint16 port;
  QString name;
  QSslCertificate cert;
};
}  // namespace srilakshmikanthanp::types::device
