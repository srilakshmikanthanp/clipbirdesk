#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QHostAddress>
#include <QHostInfo>
#include <QVBoxLayout>
#include <QWidget>
#include <tuple>

#include "ui/gui/components/core/button.hpp"
#include "ui/gui/components/core/label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class HostView : public QWidget {
 private:  // constant values for text
  const QString connect = "Connect", disconnect = "Disconnect";

 private:  // Constants for style
  static constexpr const char *const style = R"(
  )";

 public:  // enum for action
  enum class Action { Connect, Disconnect };

 signals:  // Signals
  void onAction(std::tuple<QHostAddress, quint16, Action>);

 private:  // Member variable
  QHostAddress address;
  quint16 port;
  Action action;

 private:  // Member variable
  core::Label *hostName = new core::Label(this);
  core::Label *ip = new core::Label(this);
  core::Button *actBtn = new core::Button(this);

 private:  // just for Qt meta object
  Q_OBJECT

 public:  // public Member functions
  /**
   * @brief Construct a new HostView object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit HostView(QWidget *parent = nullptr);

  /**
   * @brief Set the Host
   *
   * @param QPair<QHostAddress, quint16>
   */
  void setHost(std::tuple<QHostAddress, quint16, Action> host);

  /**
   * @brief Get the Host
   *
   * @return QPair<QHostAddress, quint16>
   */
  std::tuple<QHostAddress, quint16, Action> getHost() const;

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(HostView)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
