#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>

// Local headers
#include "types/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::mdns {
class Browser : public QObject {
 protected:

  QString m_serviceName, m_serviceType;

 private:

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(Browser)

 public:

  /**
   * @brief Construct a new Register object
   */
  Browser(QString serviceName, QString serviceType, QObject *parent = nullptr);

  /**
   * @brief Destroy the Discovery Browser object
   */
  virtual ~Browser();

  /**
   * @brief Starts the mDNS Browsing
   */
  virtual void startBrowsing() = 0;

  /**
   * @brief Stop the mDNS Browsing
   */
  virtual void stopBrowsing()  = 0;

  /**
   * @brief On server found abstract function that
   * is called when the server is found
   *
   * @param host Host address
   * @param port Port number
   */
  virtual void onServiceAdded(types::Device)   = 0;

  /**
   * @brief On Server Removed abstract function that
   * is called when the server is removed
   *
   * @param host Host address
   * @param port Port number
   */
  virtual void onServiceRemoved(types::Device) = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::mdns
