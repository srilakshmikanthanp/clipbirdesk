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
 * @brief Abstract Interface Responsible for mDNS Browsing
 */
class ImDNSBrowser : public QObject {
 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 private:

  Q_OBJECT

 public:

  /**
   * @brief Constructor
   */
  explicit ImDNSBrowser(QObject* parent) : QObject(parent) {}

  /**
   * @brief Destructor
   */
  virtual ~ImDNSBrowser()                                    = default;

  /**
   * @brief This Function Should be called to start the mDNS
   * Browsing
   */
  virtual void startBrowsing()                               = 0;

  /**
   * @brief This Function Should be called to stop the mDNS
   * Browsing
   */
  virtual void stopBrowsing()                                = 0;

  /**
   * @brief On server found abstract function that
   * is called when the server is found
   *
   * @param host Host address
   * @param port Port number
   */
  virtual void onServerAdded(QPair<QHostAddress, quint16>)   = 0;

  /**
   * @brief On Server Removed abstract function that
   * is called when the server is removed
   *
   * @param host Host address
   * @param port Port number
   */
  virtual void onServerRemoved(QPair<QHostAddress, quint16>) = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::interfaces
