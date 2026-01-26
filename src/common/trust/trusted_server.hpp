#pragma once

#include <QString>
#include <QByteArray>

namespace srilakshmikanthanp::clipbirdesk::common::trust {
struct TrustedServer {
  QString name;
  QByteArray certificate;
};
}  // namespace srilakshmikanthanp::clipbirdesk::common::trust
