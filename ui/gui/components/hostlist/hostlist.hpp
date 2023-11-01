#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWidget>

// Project headers
#include "ui/gui/components/device/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::content {
class HostList : public QWidget {
 signals:  // Signals
  void onAction(components::Device::Value host);

 private:  // just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HostList)

 public:  // Type alias

  using Action = components::Device::Action;

 private:  // Member Variables

  QStackedLayout* stackLayout = new QStackedLayout(this);
  QWidget* verticalWidget     = new QWidget(this);
  QVBoxLayout* verticalLayout = new QVBoxLayout(verticalWidget);

 public:  // Member Functions

  explicit HostList(QWidget* parent = nullptr);

  /**
   * @brief Set the Hosts to the list
   */
  void setHosts(QList<components::Device::Value> hosts);

  /**
   * @brief Get the All Hosts from the list
   */
  QList<components::Device::Value> getHosts();

  /**
   * @brief Remove all Hosts from the list
   */
  void removeHosts();

  /**
   * @brief Add Device to the list
   */
  void addHost(components::Device::Value host);

  /**
   * @brief Remove a Device from the list
   */
  void removeHost(components::Device::Value host);

 protected:  // Member Functions

  /**
   * @brief Override teh show event
   */
  void paintEvent(QPaintEvent* event) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
