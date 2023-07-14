#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/gui/components/individual/Button.hpp"
#include "ui/gui/components/individual/Label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Host : public QWidget {
 private:  // constant values
  const QString connect = "Connect", disconnect = "Disconnect";

 private:  // Member variable
  core::Label *hostname;
  core::Label *ip;
  core::Button *action;

 private:  // just for Qt
  Q_OBJECT

 public:
  enum class Action { Connect, Disconnect };

 signals:  // Signals
  void onActionClicked(Action action);

 public:
  /**
   * @brief Construct a new HostView object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Host(QWidget *parent = nullptr) : QWidget(parent) {
    // connect the button signal to this signal
    QObject::connect(action, &core::Button::clicked, [this]() {
      emit onActionClicked(this->getAction());
    });

    // create a layout to align the widgets
    QHBoxLayout *layout = new QHBoxLayout();

    // create a vertical layout
    QVBoxLayout *left = new QVBoxLayout();

    // add the hostname and ip
    left->addWidget(this->hostname);
    left->addWidget(this->ip);

    // create a vertical layout
    QVBoxLayout *right = new QVBoxLayout();

    // add the action to the right layout
    right->addWidget(this->action);

    // add the left and right layout
    layout->addLayout(left);
    layout->addLayout(right);

    // align the left and right layout
    layout->setAlignment(left, Qt::AlignLeft);
    layout->setAlignment(right, Qt::AlignRight);

    // TODO: set style sheet
  }

  /**
   * @brief Destroy the HostView object
   * with virtual destructor
   */
  ~Host() override = default;

  /**
   * @brief set the Hostname of the HostView
   *
   * @param hostname
   */
  void setHostname(const QString &hostname) {
    this->hostname->setText(hostname);
  }

  /**
   * @brief set the Ip of the HostView
   *
   * @param ip
   */
  void setIp(const QString &ip) {
    this->ip->setText(ip);
  }

  /**
   * @brief set the Action of the HostView
   *
   * @param action
   */
  void setAction(Action action) {
    if (action == Action::Disconnect) {
      this->action->setText(this->disconnect);
    } else {
      this->action->setText(this->connect);
    }
  }

  /**
   * @brief get the Hostname of the HostView
   */
  QString getHostname() const {
    return hostname->text();
  }

  /**
   * @brief get the Ip of the HostView
   */
  QString getIp() const {
    return ip->text();
  }

  /**
   * @brief get the Action of the HostView
   */
  Action getAction() const {
    if (action->text() == this->disconnect) {
      return Action::Disconnect;
    } else {
      return Action::Connect;
    }
  }

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(Host)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
