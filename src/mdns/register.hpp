#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>

namespace srilakshmikanthanp::clipbirdesk::mdns {
/**
 * @brief Abstract Discovery server that Listens for the client that send
 * the broadcast message The user of this class should implement the
 * getIpType(), getIPAddress() and getPort() functions to return the
 * IP type, IP address and port number respectively
 */
class Register : public QObject {
 protected:

  QString m_serviceName, m_serviceType;

 private:

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(Register)

 public:

  /**
   * @brief Construct a new Register object
   */
  Register(QString serviceName, QString serviceType, QObject *parent = nullptr);

  /**
   * @brief Destructor
   */
  virtual ~Register();

  /**
   * @brief Register the service
   */
  virtual void registerService(int port) = 0;

  /**
   * @brief unregister the service
   */
  virtual void unregisterService()       = 0;

  /**
   * @brief Signal for service registered
   */
  virtual void OnServiceRegistered()     = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::mdns
