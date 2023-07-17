#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHostAddress>
#include <QScrollArea>
#include <QWidget>

#include "ui/gui/components/host/host.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class HostsList : public QWidget {
 signals:  // Signals
  void onAction(Host::Value host);

 private:  // Constants for style

  static constexpr const char *const style = R"(
  )";

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HostsList)

 private:  // just for Qt

  Q_OBJECT

 public:  // Type alias

  using Action = components::Host::Action;

 private:  // Member Variables

  QVBoxLayout *verticalLayout = new QVBoxLayout(this);

 public:  // Member Functions

  explicit HostsList(QWidget *parent = nullptr);

  /**
   * @brief Set the Hosts to the list
   */
  void setHosts(QList<Host::Value> hosts);
  /**
   * @brief Get the All Hosts from the list
   */
  QList<Host::Value> getAllHosts();

  /**
   * @brief Add Host to the list
   */
  void addHost(Host::Value host);

  /**
   * @brief Remove a Host from the list
   */
  void removeHost(Host::Value host);

  /**
   * @brief Remove all Hosts from the list
   */
  void removeAllHosts();
};
#undef CAST
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
