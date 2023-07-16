#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHostAddress>
#include <QScrollArea>
#include <QWidget>

#include "ui/gui/components/HostView.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
#define CAST(x) dynamic_cast<components::HostView *>(x)
class HostsList : public QScrollArea {
 signals:  // Signals
  void onAction(std::tuple<QHostAddress, quint16, components::HostView::Action>);

 private:  // Constants for style
  static constexpr const char *const style = R"(
  )";

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(HostsList)

 private:  // just for Qt
  Q_OBJECT

 public:  // Type alias
  using Action = components::HostView::Action;

 private:  // Member Variables
  QVBoxLayout *verticalLayout = new QVBoxLayout(this);
  QWidget *widget = new QWidget(this);

 public:  // Member Functions
  explicit HostsList(QWidget *parent = nullptr) : QScrollArea(parent) {
    // set the root layout for this widget
    widget->setLayout(verticalLayout);

    // set the scroll area
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWidgetResizable(true);
    this->setWidget(this);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // add the widget to the this
    this->setWidget(widget);

    // set the style
    this->setStyleSheet(style);
  }

  /**
   * @brief Set the Hosts to the list
   */
  void setHosts(QList<std::tuple<QHostAddress, quint16, Action>> hosts) {
    // remove & add all the hosts
    removeAllHosts(); for (auto host : hosts) addHost(host);
  }

  /**
   * @brief Get the All Hosts from the list
   */
  QList<std::tuple<QHostAddress, quint16, Action>> getAllHosts() {
    // create a list of hosts
    QList<std::tuple<QHostAddress, quint16, Action>> hosts;

    // iterate over the layout
    for (int i = 0; i < verticalLayout->count(); i++) {
      auto currItem = verticalLayout->itemAt(i)->widget();
      auto hostView = CAST(currItem);
      hosts.append(hostView->getHost());
    }

    // return the list of hosts
    return hosts;
  }

  /**
   * @brief Add Host to the list
   */
  void addHost(std::tuple<QHostAddress, quint16, Action> host) {
    // create a new host view
    auto hostView = new components::HostView(this);

    // set the host
    hostView->setHost(host);

    // connect the host view signal to this signal
    QObject::connect(hostView, &HostView::onAction, [&](auto h) {
      emit onAction(h);
    });

    // add the host view to the layout
    verticalLayout->addWidget(hostView);
  }

  /**
   * @brief Remove a Host from the list
   */
  void removeHost(std::tuple<QHostAddress, quint16, Action> host) {
    // iterate over the layout
    for (int i = 0; i < verticalLayout->count(); i++) {
      auto currItem = verticalLayout->itemAt(i)->widget();
      auto hostView = CAST(currItem);
      if (hostView->getHost() != host) continue;
      verticalLayout->removeWidget(hostView);
    }
  }

  /**
   * @brief Remove all Hosts from the list
   */
  void removeAllHosts() {
    // iterate over the layout
    for (int i = 0; i < verticalLayout->count(); i++) {
      auto currItem = verticalLayout->itemAt(i)->widget();
      auto hostView = CAST(currItem);
      verticalLayout->removeWidget(hostView);
    }
  }
};
#undef CAST
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
