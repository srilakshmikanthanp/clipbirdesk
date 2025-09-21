#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// project headers
#include "controller/controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class LanController : public Controller {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(LanController)

 public:  // Constructors and Destructors

  LanController(QSslConfiguration config, QObject *parent = nullptr);
  virtual ~LanController() override;

  //----------------------- client Signals -------------------------//

 signals:  // signals for this class
  /// @brief On Server List Changed (From Client)
  void OnServerListChanged(std::optional<types::Device> server, QList<types::Device> servers);

 signals:  // signals for this class
  /// @brief On Server Found  (From Client)
  void OnServerFound(types::Device server);

 signals:  // signals for this class
  /// @brief On Server Gone
  void OnServerGone(types::Device);

 signals:  // signals for this class
  /// @brief On Connection Error
  void OnConnectionError(QString error);

 signals:  // signals for this class
  /// @brief On Server state changed (From Client)
  void OnServerStatusChanged(bool isConnected, types::Device);

  //----------------------- server Signals ------------------------//

 signals:  // signals for this class
  /// @brief On Server state changed (From Server)
  void OnMdnsRegisterStatusChangeChanged(bool isRegistered);

 signals:  // signals
  /// @brief On client state changed (From Server)
  void OnCLientStateChanged(types::Device client, bool connected);

 signals:  // signals for this class
  /// @brief On Sync Request
  void OnAuthRequest(types::Device client);

 signals:  // signals for this class
  /// @brief On Sync Request  (From Server)
  void OnClientListChanged(QList<types::Device> clients);

  //----------------------- Common Signals ------------------------//

 signals:  // signals for this class
  /// @brief On Sync Request  (From Server)
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> data);

 signals:  // signals for this class
  /// @brief On Host Type Changed
  void OnHostTypeChanged(types::enums::HostType);

 private:  // typedefs for this class

  using Server = syncing::lan::Server;
  using Client = syncing::lan::Client;

 private:  // Member variable

  std::variant<Server, Client> m_host;
  QSslConfiguration m_sslConfig;

 public:  // Member functions
  void synchronize(const QVector<QPair<QString, QByteArray>> &data);
  QSslConfiguration getSslConfiguration() const;
  void setSslConfiguration(const QSslConfiguration &config);
  std::variant<Server, Client>& getHost();
  types::enums::HostType getHostType() const;
  void setHostAsServer();
  void setHostAsClient();
  void authSuccess(const types::Device &client);
  void authFailed(const types::Device &client);
  void disposeServer();
  void disposeClient();
};
}  // namespace srilakshmikanthanp::clipbirdesk::controller
