// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QSettings>

namespace srilakshmikanthanp::clipbirdesk::storage {
class Storage : public QObject {
 private:  // variables

  QSettings *settings = new QSettings("srilakshmikanthanp", "clipbird");
  const char* clientGroup = "client";
  const char* serverGroup = "server";

 private:  // qt

  Q_OBJECT

 public:  // constructor

  /**
   * @brief Construct a new SQLStore object
   *
   * @param parent
   */
  Storage(QObject *parent = nullptr);

  /**
   * @brief Destroy the SQLStore object
   */
  virtual ~Storage() = default;

  /**
   * @brief Store Client hostname and JWT token
   *
   * @param hostname
   * @param token
   */
  void setClientToken(const QString &hostname, const QString &token);

  /**
   * @brief Get the JWT token for the hostname
   *
   * @param hostname
   * @return QString
   *
   * @throw std::invalid_argument if hostname not found
   */
  QString getClientToken(const QString &hostname);

  /**
   * @brief Store the server hostname and JWT token
   *
   * @param hostname
   * @param token
   */
  void setServerToken(const QString &hostname, const QString &token);

  /**
   * @brief Get the JWT token for the hostname
   *
   * @param hostname
   * @return QString
   *
   * @throw std::invalid_argument if hostname not found
   */
  QString getServerToken(const QString &hostname);
};

}  // namespace srilakshmikanthanp::clipbirdesk::storage
