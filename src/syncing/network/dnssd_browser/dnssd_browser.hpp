#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__) // Only for Operating System that supports bonjour or compatible to it
#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// C++ headers
#include <algorithm>
#include <vector>

// Qt headers
#include <QByteArray>
#include <QHostAddress>
#include <QHostInfo>
#include <QObject>
#include <QRegularExpression>
#include <QSocketNotifier>
#include <QTimer>
#include <QUdpSocket>
#include <QtLogging>
#include <QtTypes>
#include <QMutex>
#include <QDebug>

// htons, ntohs
#ifdef __linux__
  #include <arpa/inet.h>
#endif

// mDNS headers
#include <dns_sd.h>

// Local headers
#include "syncing/network/net_browser.hpp"
#include "constants/constants.hpp"
#include "common/types/enums/enums.hpp"
#include "syncing/network/net_resolved_device.hpp"
#include "utility/functions/ipconv/ipconv.hpp"

// Platform headers
#include "utility/functions/socket/socket.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
/**
 * @brief Discovery client that sends the broadcast message
 * to the server and listen for the response if any server
 * is found then the callback function is called
 */
class DnssdBrowser : public NetBrowser {
 private:  // private variables

  QSocketNotifier* m_browse_notify = nullptr;  ///< Socket notifier
  DNSServiceRef m_browse_ref       = nullptr;  ///< Service ref
  QSocketNotifier* m_res_notify    = nullptr;  ///< Socket notifier
  DNSServiceRef m_res_ref          = nullptr;  ///< Service ref

 private:  // private variables

  QMap<QString, QPair<QHostAddress, quint16>> serviceMap;

 private:  // Just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DnssdBrowser)

 private:  // private functions

  /**
   * @brief Callback for QHostInfo Address Resolve Function
   *
   * @param isAdded
   * @param const QHostInfo& info
   */
  void onHostResolved(quint16 port, QString srvName, const QHostInfo& info);

  /**
   * @brief Callback function for DNSServiceBrowse function
   */
  static void browseCallback(
      DNSServiceRef serviceRef,       // DNSServiceRef
      DNSServiceFlags flags,          // DNSServiceFlags
      uint32_t interfaceIndex,        // InterfaceIndex
      DNSServiceErrorType errorCode,  // DNSServiceErrorType
      const char* serviceName,        // serviceName
      const char* regtype,            // regtype
      const char* domain,             // domain
      void* context                   // context
  );

  /**
   * @brief Callback function for DNSServiceResolve function
   * that service Added
   */
  static void addedCallback(
      DNSServiceRef serviceRef,        // DNSServiceRef
      DNSServiceFlags flags,           // DNSServiceFlags
      uint32_t interfaceIndex,         // InterfaceIndex
      DNSServiceErrorType errorCode,   // DNSServiceErrorType
      const char* fullname,            // fullname
      const char* hosttarget,          // hosttarget
      uint16_t port,                   // port
      uint16_t txtLen,                 // txtLen
      const unsigned char* txtRecord,  // txtRecord
      void* context                    // context
  );

  /**
   * @brief Callback function for DNSServiceResolve function
   * that service Removed
   */
  void removeCallback(QString serviceName);

 public:

  /**
   * @brief Construct a new Discovery MdnsBrowser object
   *
   * @param parent Parent object
   */
  explicit DnssdBrowser(QObject* parent = nullptr);

  /**
   * @brief Destroy the Discovery MdnsBrowser object
   */
  virtual ~DnssdBrowser();

  /**
   * @brief Starts the mDNS Browsing
   */
  void start();

  /**
   * @brief Stop the mDNS Browsing
   */
  void stop();
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::dnsd
#endif
