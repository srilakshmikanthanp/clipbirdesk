#pragma once

#include <QString>
#include <QByteArray>

namespace srilakshmikanthanp::clipbirdesk::common::trust {
struct TrustedClient {
  QString name;
  QByteArray certificate;
};
}  // namespace srilakshmikanthanp::clipbirdesk::common::trust
