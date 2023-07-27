#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFormLayout>
#include <QPair>
#include <QWidget>
#include <QLabel>

#include "ui/gui/components/status/status.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::window {
class DeviceInfo : public QWidget {
 private:  // Member Variables

  QPair<QLabel*, components::Status*> hostStatus;
  QPair<QLabel*, QLabel*> serverName;
  QPair<QLabel*, QLabel*> serverIp;
  QPair<QLabel*, QLabel*> hostCount;

 private:  // just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DeviceInfo)

 public:  // Member Functions
  /**
   * @brief Construct a new Device Info object
   *
   * @param parent
   */
  explicit DeviceInfo(QWidget* parent = nullptr);

  /**
   * @brief Set the Host Label and Status
   *
   * @param QPair
   */
  void setHostStatus(const QPair<QString, components::Status::Value>&);

  /**
   * @brief Set the Server Name
   *
   * @param QPair
   */
  void setServerName(const QPair<QString, QString>&);

  /**
   * @brief Set the Server Ip
   *
   * @param QPair
   */
  void setServerIpPort(const QPair<QString, QString>&);

  /**
   * @brief Set the Host Count
   *
   * @param QPair
   */
  void setHostCount(const QPair<QString, QString>&);

  /**
   * @brief Get the Host Label and Status
   *
   * @return QPair
   */
  QPair<QString, components::Status::Value> getHostStatus() const;

  /**
   * @brief Get the Server Name
   *
   * @return QPair
   */
  QPair<QString, QString> getServerName() const;

  /**
   * @brief Get the Server Ip
   *
   * @return QPair
   */
  QPair<QString, QString> getServerIpPort() const;

  /**
   * @brief Get the Host Count
   *
   * @return QPair
   */
  QPair<QString, QString> getHostCount() const;

  /**
   * @brief Destroy the Device Info object
   */
  ~DeviceInfo() override = default;
};
}
