// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QSettings>

namespace srilakshmikanthanp::clipbirdesk::storage {
class Storage : public QObject {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "clipbird");

 private: // groups
  const char* clientGroup  = "client";
  const char* generalGroup = "general";
  const char* serverGroup  = "server";

 private: // keys
  const char* hostStateKey = "hostState";

 private:  // qt

  Q_OBJECT

 public:  // constructor

  /**
   * @brief Construct a new SQLStore object
   */
  Storage(QObject *parent = nullptr);

  /**
   * @brief Destroy the SQLStore object
   */
  virtual ~Storage() = default;

  /**
   * @brief Store Client hostname and JWT cert
   */
  void setClientCert(const QString &hostname, const QString &cert);

  /**
   * @brief has the cert for the hostname
   */
  bool hasClientCert(const QString &hostname);

  /**
   * @brief Get the JWT cert for the hostname
   */
  QString getClientCert(const QString &hostname);

  /**
   * @brief Store the server hostname and JWT cert
   */
  void setServerCert(const QString &hostname, const QString &cert);

  /**
   * @brief has the cert for the hostname
   */
  bool hasServerCert(const QString &hostname);

  /**
   * @brief Get the JWT cert for the hostname
   */
  QString getServerCert(const QString &hostname);

  /**
   * @brief Set the current state of the server or client
   */
  void setHostIsServer(bool isServer);

  /**
   * @brief Get the current state of the server or client
   */
  bool getHostIsServer();
};

}  // namespace srilakshmikanthanp::clipbirdesk::storage
