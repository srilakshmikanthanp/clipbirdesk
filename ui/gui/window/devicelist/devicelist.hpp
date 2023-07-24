#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QWidget>

// Project headers
#include "ui/gui/components/host/host.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::window {
class DeviceList : public QWidget {
 signals:  // Signals
  void onAction(components::Host::Value host);

 private:  // just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DeviceList)

 public:  // Type alias

  using Action = components::Host::Action;

 private:  // Member Variables

  QVBoxLayout* verticalLayout = new QVBoxLayout();

 public:  // Member Functions

  explicit DeviceList(QWidget* parent = nullptr);

  /**
   * @brief Set the Hosts to the list
   */
  void setHosts(QList<components::Host::Value> hosts);

  /**
   * @brief Get the All Hosts from the list
   */
  QList<components::Host::Value> getHosts();

  /**
   * @brief Remove all Hosts from the list
   */
  void removeHosts();

  /**
   * @brief Add Host to the list
   */
  void addHost(components::Host::Value host);

  /**
   * @brief Remove a Host from the list
   */
  void removeHost(components::Host::Value host);
};
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
