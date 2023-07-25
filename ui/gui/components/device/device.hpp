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
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>


// C++ Headers
#include <tuple>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Device : public QWidget {
 private:  // constant values for text

  const QString connect = "Connect", disconnect = "Disconnect";

 public:  // enum for action

  enum class Action { Connect, Disconnect };

 public:  // typedefs

  using Value = std::tuple<QHostAddress, quint16, Action>;

 signals:  // Signals
  void onAction(const std::tuple<QHostAddress, quint16, Action>&);

 private:  // Private Functions

  void onHostResolved(const QHostInfo &info);

 private:  // Member variable

  QHostAddress address;
  quint16 port;
  Action action;

 private:  // Member variable

  QLabel *hostName = new QLabel(this);
  QPushButton *actBtn  = new QPushButton(this);

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
   *
   * @param QPair<QHostAddress, quint16>
   */
  void setHost(Value host);

  /**
   * @brief Get the Host
   *
   * @return QPair<QHostAddress, quint16>
   */
  Value getHost() const;

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Device)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
