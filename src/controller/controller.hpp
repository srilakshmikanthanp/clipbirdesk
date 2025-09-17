#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QHostInfo>
#include <QObject>
#include <QSslConfiguration>
#include <QSslSocket>

// C++ headers
#include <functional>
#include <variant>

// project headers
#include "clipboard/applicationclipboard.hpp"
#include "syncing/synchronizer.hpp"
#include "syncing/lan/client/client.hpp"
#include "syncing/lan/server/server.hpp"
#include "syncing/wan/hub/hub_websocket.hpp"
#include "store/storage.hpp"
#include "types/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class Controller : public QObject {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(Controller)

 public:  // Constructors and Destructors

  Controller(QObject *parent = nullptr);
  virtual ~Controller() override;
};
}
