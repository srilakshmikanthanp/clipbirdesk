#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFormLayout>
#include <QStyleOption>
#include <QPair>
#include <QWidget>
#include <QPaintEvent>
#include <QLabel>

#include "ui/gui/components/status/status.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::content {
class DeviceInfo : public QWidget {
 private:  // Member Variables

  QPair<QLabel*, QLabel*> groupName;
  QPair<QLabel*, components::Status*> hostStatus;
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
  void setGroupName(const QPair<QString, QString>&);

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
  QPair<QString, QString> getGroupName() const;

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
