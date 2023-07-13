#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "./core/Button.hpp"
#include "./core/Label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class HostView : public QWidget {
 private:  // Member variable
  core::Label *hostname;
  core::Label *ip;
  core::Button *action;

 signals:  // Signals
  void onDisconnectClicked();

 signals:  // Signals
  void onConnectClicked();

 private:  // just for Qt
  Q_OBJECT

 private:
  /**
   * @brief onConnectClicked signal is called
   * when the connect button is clicked
   */
  void onConnectClicked() {
    if (action->text().toLower() == "connect") {
      emit onConnectClicked();
    } else {
      emit onDisconnectClicked();
    }
  }

 public:
  /**
   * @brief Construct a new HostView object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit HostView(QWidget *parent = nullptr) : QWidget(parent) {
    // create the Vertical layout
    auto *v_layout_l = new QVBoxLayout(this);

    // Add hostname and ip
    v_layout_l->addWidget(hostname = new core::Label(this));
    v_layout_l->addWidget(ip = new core::Label(this));

    // create the Vertical layout
    auto *v_layout_r = new QVBoxLayout(this);

    // Add action
    v_layout_r->addWidget(action = new core::Button(this));

    // create the Horizontal layout
    auto *h_layout = new QHBoxLayout();

    // Add the vertical layout to horizontal layout in left
    h_layout->addWidget(v_layout_l, 0, Qt::AlignLeft);

    // Add the vertical layout to horizontal layout in right
    h_layout->addWidget(v_layout_r, 0, Qt::AlignRight);

    // set the layout
    this->setLayout(h_layout);

    // connect the button signal to this signal
    const auto signal = &core::Button::clicked;
    const auto slot = &HostView::onConnectClicked;
    connect(action, signal, this, slot);

    // TODO: set style sheet
  }

  /**
   * @brief set the Hostname of the HostView
   */
  void setHostname(const QString &hostname) {
    this->hostname->setText(hostname);
  }

  /**
   * @brief set the Ip of the HostView
   */
  void setIp(const QString &ip) {
    this->ip->setText(ip);
  }

  /**
   * @brief set the Action of the HostView
   */
  void setAction(bool action) {
    this->action->setText(action ? "Connect" : "Disconnect");
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
  bool getAction() const {
    return action->text().toLower() == "connect";
  }

  /**
   * @brief Destroy the HostView object
   * with virtual destructor
   */
  ~HostView() override = default;

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(HostView)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
