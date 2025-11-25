#pragma once

#include <QFuture>
#include <QObject>
#include <QString>

#include "packets/network_packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class Session : public QObject {
  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(Session)

 private:

  QString name;

 public:

  explicit Session(const QString &name, QObject *parent = nullptr) : QObject(parent), name(name) {}
  virtual ~Session()                                            = default;

  virtual void sendPacket(const packets::NetworkPacket &packet) = 0;
  virtual void disconnect()                                     = 0;
  virtual bool isTrusted() const                                = 0;
  virtual QByteArray getCertificate() const                     = 0;

  QString getName() const;

  bool operator==(const Session &other) const;
  bool operator!=(const Session &other) const;

 signals:
  void onTrustedStateChanged(bool isTrusted);
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing

template <>
struct std::hash<srilakshmikanthanp::clipbirdesk::syncing::Session> {
  std::size_t operator()(const srilakshmikanthanp::clipbirdesk::syncing::Session &session) const noexcept {
    return std::hash<QString>()(session.getName());
  }
};
