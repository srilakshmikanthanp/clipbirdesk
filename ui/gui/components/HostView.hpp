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

#include "ui/gui/components/core/Button.hpp"
#include "ui/gui/components/core/Label.hpp"

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
  explicit HostView(QWidget *parent = nullptr) : QWidget(parent) {
    // connect the button signal to this signal
    QObject::connect(actBtn, &core::Button::clicked, [this]() {
      emit onAction(this->getHost());
    });

    // create a layout to align the widgets
    QHBoxLayout *layout = new QHBoxLayout();

    // create a vertical layout
    QVBoxLayout *left = new QVBoxLayout();

    // add the hostname and ip
    left->addWidget(this->hostName);
    left->addWidget(this->ip);

    // create a vertical layout
    QVBoxLayout *right = new QVBoxLayout();

    // add the action to the right layout
    right->addWidget(this->actBtn);

    // add the left and right layout
    layout->addLayout(left);
    layout->addLayout(right);

    // align the left and right layout
    layout->setAlignment(left, Qt::AlignLeft);
    layout->setAlignment(right, Qt::AlignRight);

    this->setStyleSheet(style);
  }

  /**
   * @brief Set the Host
   *
   * @param QPair<QHostAddress, quint16>
   */
  void setHost(std::tuple<QHostAddress, quint16, Action> host) {
    // set the address and port
    this->address = std::get<0>(host);
    this->port = std::get<1>(host);
    this->action = std::get<2>(host);

    // get the host info
    const auto info = QHostInfo::fromName(address.toString());

    // set the host name
    this->hostName->setText(info.hostName());

    // set the ip
    this->ip->setText(address.toString());

    // action text to set
    const auto a = action == Action::Disconnect ? disconnect : connect;

    // set the action
    this->actBtn->setText(a);
  }

  /**
   * @brief Get the Host
   *
   * @return QPair<QHostAddress, quint16>
   */
  std::tuple<QHostAddress, quint16, Action> getHost() const {
    return std::make_tuple(address, port, action);
  }

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(HostView)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
