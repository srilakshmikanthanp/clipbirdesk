#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QHostAddress>
#include <QHostInfo>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyle>
#include <QStyleOption>

// C++ Headers
#include <tuple>

// local headers
#include "types/device/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Host : public QWidget {
 private:  // constant values for text

  const QString connect = "Connect", disconnect = "Disconnect";

 public:  // enum for action

  enum class Action { Connect, Disconnect };

 public:  // typedefs

  using Value = std::tuple<types::device::Device, Action>;

 signals:  // Signals
  void onAction(std::tuple<types::device::Device, Action>);

 private:  // Member variable

  types::device::Device device;
  Action action;

 private:  // Member variable

  QPushButton *actBtn  = new QPushButton(this);
  QLabel *hostName = new QLabel(this);

 private:  // just for Qt meta object

  Q_OBJECT

 public:  // public Member functions

  /**
   * @brief Construct a new HostView object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Host(QWidget *parent = nullptr);

  /**
   * @brief Set the Host
   */
  void setHost(Value host);

  /**
   * @brief Get the Host
   */
  Value getHost() const;

  /**
   * @brief Override paint for custom style
   */
  void paintEvent(QPaintEvent *event) override;

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Host)
};

class HostList : public QWidget {
 signals:  // Signals
  void onAction(components::Host::Value host);

 private:  // just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HostList)

 public:  // Type alias

  using Action = components::Host::Action;

 private:  // Member Variables

  QStackedLayout* stackLayout = new QStackedLayout(this);
  QWidget* verticalWidget     = new QWidget(this);
  QVBoxLayout* verticalLayout = new QVBoxLayout(verticalWidget);

 public:  // Member Functions

  explicit HostList(QWidget* parent = nullptr);

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
   * @brief Add Device to the list
   */
  void addHost(components::Host::Value host);

  /**
   * @brief Remove a Device from the list
   */
  void removeHost(components::Host::Value host);

 protected:  // Member Functions

  /**
   * @brief Override teh show event
   */
  void paintEvent(QPaintEvent* event) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
