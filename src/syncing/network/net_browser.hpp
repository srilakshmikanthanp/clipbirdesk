#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>

// Local headers
#include "net_resolved_device.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
class NetBrowser : public QObject {
 private:

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(NetBrowser)

 public:

  /**
   * @brief Construct a new Register object
   */
  NetBrowser(QObject *parent = nullptr);

  /**
   * @brief Destroy the Discovery Browser object
   */
  virtual ~NetBrowser();

  /**
   * @brief Starts the mDNS Browsing
   */
  virtual void start()                                = 0;

  /**
   * @brief Stop the mDNS Browsing
   */
  virtual void stop()                                 = 0;

 signals:
  void onServiceAdded(NetResolvedDevice);
  void onServiceRemoved(NetResolvedDevice);
  void onBrowsingStarted();
  void onBrowsingStopped();
  void onBrowsingStartFailed(std::exception_ptr eptr);
  void onBrowsingStopFailed(std::exception_ptr eptr);
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
