#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__)   // Only for Operating System that supports bonjour or compatible to its
#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// C++ headers
#include <algorithm>
#include <vector>

// Qt headers
#include <QByteArray>
#include <QHostAddress>
#include <QHostInfo>
#include <QObject>
#include <QSocketNotifier>
#include <QTimer>
#include <QUdpSocket>
#include <QtLogging>
#include <QDebug>

// htons, ntohs
#ifdef __linux__
  #include <arpa/inet.h>
#endif

// mDNS headers
#include <dns_sd.h>

// Local headers
#include "mdns/register.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"

namespace srilakshmikanthanp::clipbirdesk::mdns {
/**
 * @brief Abstract Discovery server that Listens for the client that send
 * the broadcast message The user of this class should implement the
 * getIpType(), getIPAddress() and getPort() functions to return the
 * IP type, IP address and port number respectively
 */
class DnssdRegister : public Register {
 private:   // private variables

  QSocketNotifier* m_notifier = nullptr; ///< Socket notifier
  DNSServiceRef m_serviceRef = nullptr;  ///< Service ref

 signals:
  // Signal for service registered
  void OnServiceRegistered() override;

 private:  // typedefs for this class

  using IPType = types::enums::IPType;

 private:  // Just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DnssdRegister)

 private: // private functions

  /**
   * @brief Callback function for DNSServiceRegister
   */
  static void publishCallback(
    DNSServiceRef serviceRef,       // DNSServiceRef
    DNSServiceFlags flags,          // DNSServiceFlags
    DNSServiceErrorType errorCode,  // DNSServiceErrorType
    const char* name,               // name of service
    const char* regtype,            // regtype
    const char* domain,             // domain
    void* context                   // context
  );

  /**
   * @brief Process Activated
   */
  void processActivated();

 public:  // public functions

  /**
   * @brief Construct a new Discovery Register object
   *
   * @param parent Parent object
   */
  explicit DnssdRegister(QString serviceName, QString serviceType, QObject* parent = nullptr);

  /**
   * @brief Destroy the Discovery Register object
   */
  virtual ~DnssdRegister();

 public:

  /**
   * @brief Register the service
   */
  void registerService(int port);

  /**
   * @brief Stop the server
   */
  void unregisterService();
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::service
#endif
