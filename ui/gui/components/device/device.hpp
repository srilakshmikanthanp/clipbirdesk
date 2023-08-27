#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QHBoxLayout>
#include <QHostAddress>
#include <QHostInfo>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyle>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QWidget>

// C++ Headers
#include <tuple>

// local headers
#include "types/device/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Device : public QWidget {
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
  explicit Device(QWidget *parent = nullptr);

  /**
   * @brief Set the Host
   */
  void setHost(Value host);

  /**
   * @brief Get the Host
   */
  Value getHost() const;

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Device)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
