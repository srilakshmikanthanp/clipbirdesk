#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QApplication>
#include <QDesktopServices>
#include <QFocusEvent>
#include <QFormLayout>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QHostInfo>
#include <QLineEdit>
#include <QPaintEngine>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QScrollArea>
#include <QStyleHints>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QFrame>
#include <QStyleOption>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent>

// standard headers
#include <algorithm>
#include <tuple>
#include <variant>

// project headers
#include "controller/clipbird/clipbird.hpp"
#include "types/device/device.hpp"
#include "ui/gui/components/status/status.hpp"
#include "ui/gui/components/hostlist/hostlist.hpp"
#include "ui/gui/notify/joinrequest/joinrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Clipbird : public QDialog {
 private:  // Member variable (Tray)

  ui::gui::components::Status *status = new ui::gui::components::Status(this);  // Status

 private:  // Member variable (Tabs)

  components::HostList* clientList = new components::HostList();  // Server Tab
  components::HostList* serverList = new components::HostList();  // Client Tab

 private:  // Member variable (Layout)

  QTabWidget* tab = new QTabWidget();

 private:  // typedefs used in this class

  using Action = components::HostTile::Action;
  using Status = components::Status::Value;

 private:  // typedefs used in this class

  using ClipBird = controller::ClipBird;

 private:  // Disable copy and move

  Q_DISABLE_COPY_MOVE(Clipbird)

 public:  // enum for this class

  enum class Tabs { Server = 0, Client = 1 };

 private:  // Member variable

  Tabs currentTab = Tabs::Server;

 private:  // just for Qt

  Q_OBJECT

 private:  // Member Variables

  ClipBird* controller;

 signals:  // signals
  void onHostAction(Tabs tab, std::tuple<types::device::Device, Action>);

 signals:  // signals
  void onTabChanged(Tabs tab);  // emit when tab changed

 private:  // private slots

  //------------------------------ slots for Clipbird -------------------------//

  /**
   * @brief handle the host action from the window
   */
  void handleHostAction(Tabs t, std::tuple<types::device::Device, Action> h);

  //----------------------------- slots for Server --------------------------//

  /**
   * @brief Handle the Client List Item Clicked
   */
  void handleClientListChange(QList<types::device::Device> clients);

  /**
   * @brief Handle the Client Tab Change
   */
  void handleTabChangeForClient(Tabs tab);

  /**
   * @brief Handle the Server Tab Change
   */
  void handleTabChangeForServer(Tabs tab);

  /**
   * @brief Handle the Server State Change
   */
  void handleServerStateChange(bool isStarted);

  /**
   * @brief Handle the Client auth Request
   */
  void handleAuthRequest(const types::device::Device& client);

  //----------------------------- slots for Client --------------------------//

  /**
   * @brief Handle the Server List Item Clicked
   */
  void handleServerListChange(QList<types::device::Device> servers);

  /**
   * @brief Handle the server status change
   */
  void handleServerStatusChanged(bool status);

  //----------------------------- slots for Event ----------------------------//

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:

  /**
   * @brief Construct a new Clipbird object
   * with parent as QWidget
   */
  explicit Clipbird(ClipBird* controller, QWidget* parent = nullptr);

  /**
   * @brief Set the Status object
   */
  void setStatus(const QString& key, components::Status::Value val);

  /**
   * @brief Get the Host Status object
   */
  QPair<QString, components::Status::Value> getStatus();

  /**
   * @brief Set tab as client
   */
  void setTabAsClient();

  /**
   * @brief Set tab as server
   */
  void setTabAsServer();

  //---------------------- Server Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setClientList(const QList<std::tuple<types::device::Device, Action>>& hosts);

  /**
   * @brief Get the Server List object
   */
  QList<std::tuple<types::device::Device, Action>> getClientList();

  /**
   * @brief Add Server to the list
   */
  void addClient(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove a Server from the list
   */
  void removeClient(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove all servers from the list
   */
  void removeAllClient();

  //---------------------- Client Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setServerList(const QList<std::tuple<types::device::Device, Action>>& hosts);

  /**
   * @brief Get the Server List from the tab
   */
  QList<std::tuple<types::device::Device, Action>> getServerList();

  /**
   * @brief Add Server to the list
   */
  void addServer(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove a Server from the list
   */
  void removeServer(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove all servers from the list
   */
  void removeAllServers();

 protected:  // Member Functions

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;

  /**
   * @brief override set visible
   */
  void setVisible(bool visible) override;

  /**
  * @brief paint event
  */
  void paintEvent(QPaintEvent *event) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
