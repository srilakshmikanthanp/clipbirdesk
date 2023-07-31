#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHostAddress>
#include <QObject>

#include "types/enums/enums.hpp"

namespace srilakshmikanthanp::clipbirdesk::interfaces {
/**
 * @brief Abstract Discovery Interface for mDNS Register
 */
class ImDNSRegister : public QObject {
 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 signals:  // signals for this class
  /// @brief On Service Registered
  void OnServiceRegistered();

 private:  // Just For Qt

  Q_OBJECT

 public:  // Members

  /**
   * @brief Construct a new ImDNSRegister object
   *
   * @param parent Parent object
   */
  ImDNSRegister(QObject* parent = nullptr) : QObject(parent) {}

  /**
   * @brief Destroy the ImDNSRegister object
   */
  virtual ~ImDNSRegister()                        = default;

  /**
   * @brief Register the service
   *
   * @param callback Callback function to be called
   * when service Registered
   */
  virtual void registerServiceAsync()            = 0;

  /**
   * @brief Stop the server
   */
  virtual void unregisterService()               = 0;

  /**
   * @brief Get the IP Type of the server it can be IPv4 or IPv6
   * the IP type is used to determine the length of the IP address
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @note UNUSED ON Current Implementation
   * @return types::IPType IP type
   * @throw Any Exception If any error occurs
   */
  virtual types::enums::IPType getIPType() const = 0;

  /**
   * @brief Get the Port number of the server this currently
   * it is running
   *
   * @return types::Port Port number
   * @throw Any Exception If any error occurs
   */
  virtual quint16 getPort() const                = 0;

  /**
   * @brief Get the IP Address of the server it can be IPv4 or IPv6
   *
   * @note UNUSED ON Current Implementation
   * @return types::IPAddress IP address
   * @throw Any Exception If any error occurs
   */
  virtual QHostAddress getIPAddress() const      = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::interfaces
