#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QSslCertificate>
#include <QSslKey>
#include <QObject>
#include <QSettings>

#include <optional>

namespace srilakshmikanthanp::clipbirdesk::storage {
class Storage : public QObject {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "clipbird", this);

 private:  // groups

  const char *clientGroup = "client";
  const char *commonGroup = "common";
  const char *hubGroup    = "hub";
  const char *serverGroup = "server";

 private:  // keys

  const char *hostStateKey       = "hostState";
  const char *hubStateKey        = "hubState";
  const char *hostKeyKey         = "hostKey";
  const char *hostCertificateKey = "hostCert";
  const char *proxyKey           = "proxy";
  const char *easyHideKey        = "easyHide";
  const char *hubDeviceIdKey     = "hubDeviceId";

 private:  // qt

  Q_OBJECT

 private:  // constructor

  /**
   * @brief Construct a new SQLStore object
   */
  Storage(QObject *parent = nullptr);

 private:  // constructor

  Q_DISABLE_COPY_MOVE(Storage)

 public:  // methods

  /**
   * @brief Destroy the SQLStore object
   */
  virtual ~Storage() = default;

  /**
   * @brief Store Client name and JWT cert
   */
  void setClientCert(const QString &name, const QByteArray &cert);

  /**
   * @brief has the cert for the name
   */
  bool hasClientCert(const QString &name);

  /**
   * @brief Clear the client cert
   */
  void clearClientCert(const QString &name);

  /**
   * @brief Clear the client cert
   */
  void clearAllClientCert();

  /**
   * @brief Get the JWT cert for the name
   */
  QByteArray getClientCert(const QString &name);

  /**
   * @brief Get All Client Certificates
   */
  QList<QByteArray> getAllClientCert();

  /**
   * @brief Store the server name and JWT cert
   */
  void setServerCert(const QString &name, const QByteArray &cert);

  /**
   * @brief has the cert for the name
   */
  bool hasServerCert(const QString &name);

  /**
   * @brief Clear the server cert
   */
  void clearServerCert(const QString &name);

  /**
   * @brief Clear the server cert
   */
  void clearAllServerCert();

  /**
   * @brief Get the JWT cert for the name
   */
  QByteArray getServerCert(const QString &name);

  /**
   * @brief Get All Server Certificates
   */
  QList<QByteArray> getAllServerCert();

  /**
   * @brief Set the current state of the server or client
   */
  void setHostIsLastlyServer(bool isServer);

  /**
   * @brief Get the current state of the server or client
   */
  bool getHostIsLastlyServer();

  /**
   * @brief set the Host certificate
   */
  void setHostCert(const QSslCertificate &cert);

  /**
   * @brief has Host cert
   */
  bool hasHostCert();

  /**
   * @brief Get the Host certificate
   */
  QSslCertificate getHostCert();

  /**
   * @brief set the Host Key
   */
  void setHostKey(const QSslKey &key);

  /**
   * @brief has Host Key
   */
  bool hasHostKey();

  /**
   * @brief Get the Host Key
   */
  QSslKey getHostKey();

  /**
   * @brief is Hub connected lastly
   */
  bool getHubIsConnectedLastly();

  /**
   * @brief set is Hub connected lastly
   */
  void setIsConnectedToHubLastly(bool isConnected);

  /**
   * @brief Instance of the storage
   */
  static Storage &instance();
};
}  // namespace srilakshmikanthanp::clipbirdesk::storage
