#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
/**
 * @brief Abstract Discovery server that Listens for the client that send
 * the broadcast message The user of this class should implement the
 * getIpType(), getIPAddress() and getPort() functions to return the
 * IP type, IP address and port number respectively
 */
class Register : public QObject {
 private:

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(Register)

 public:
  Register(QObject *parent = nullptr);
  virtual ~Register();

  virtual void registerService(int port) = 0;
  virtual void unregisterService()       = 0;

 signals:
  void OnServiceRegistered();
  void OnServiceUnregistered();
  void OnServiceRegisteringFailed(std::exception_ptr eptr);
  void OnServiceUnregisteringFailed(std::exception_ptr eptr);
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::mdns
