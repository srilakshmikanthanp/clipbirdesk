#pragma once

#include <QObject>
#include <QString>

#include "common/types/ssl_config/ssl_config.hpp"
#include "client_server.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class ClientServerBrowser : public QObject {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(ClientServerBrowser)

 protected:
  common::types::SslConfig sslConfig;

 public:
  explicit ClientServerBrowser(const common::types::SslConfig& sslConfig, QObject* parent = nullptr);
  virtual ~ClientServerBrowser();

  virtual void start() = 0;
  virtual void stop() = 0;

 signals:
  void onBrowsingStartFailed(std::exception_ptr eptr);
  void onBrowsingStopFailed(std::exception_ptr eptr);
  void onBrowsingStarted();
  void onBrowsingStopped();
  void onServerGone(ClientServer* server);
  void onServerFound(ClientServer* server);
};
}
