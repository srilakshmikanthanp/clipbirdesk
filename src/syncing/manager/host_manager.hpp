#pragma once

#include <QObject>

#include "syncing/synchronizer.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class HostManager : public Synchronizer {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(HostManager)

 public:
  explicit HostManager(QObject *parent = nullptr);
  virtual ~HostManager();

  virtual void start(bool useBluetooth) = 0;
  virtual void stop()                   = 0;
};
}
