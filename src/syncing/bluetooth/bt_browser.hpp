#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>

// Local headers
#include "bt_resolved_device.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
class BtBrowser : public QObject {
 private:

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(BtBrowser)

 public:

  BtBrowser(QObject *parent = nullptr);
  virtual ~BtBrowser();

  virtual void start()                                        = 0;
  virtual void stop()                                         = 0;

 signals:
  void onServiceAdded(BtResolvedDevice);
  void onServiceRemoved(BtResolvedDevice);
  void onBrowsingStarted();
  void onBrowsingStopped();
  void onBrowsingStartFailed(std::exception_ptr eptr);
  void onBrowsingStopFailed(std::exception_ptr eptr);
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth
