#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QListWidget>
#include <QListWidgetItem>

#include "ui/gui/components/Host.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class List : public QListWidget {
 signals:  // Signals
  void onActionClicked(components::Host::Action action, QString host, QString ip);

 private:  // just for Qt
  Q_OBJECT

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(List)

 public:  // Type alias
  using Action = components::Host::Action;

 public:  // Member Functions
  explicit List(QWidget* parent = nullptr) : QListWidget(parent) {}

  /**
   * @brief Add Host to the list
   *
   * @param hostName
   * @param ip
   */
  void addHost(QString hostName, QString ip, Action action) {
    // create lambda for signal
    auto lambda = [this](Action action, QString host, QString ip) {
      emit onActionClicked(action, host, ip);
    };

    // create host object and set parent
    auto host = new components::Host(this);

    // set host name and ip and action
    host->setHostname(hostName);
    host->setIp(ip);
    host->setAction(action);

    // connect signal to lambda
    QObject::connect(host, &components::Host::onActionClicked, lambda);

    // create a list item
    auto listItem = new QListWidgetItem(this);

    // set the size of the item
    listItem->setSizeHint(host->sizeHint());

    // add the item to the list
    this->addItem(listItem);
  }

  /**
   * @brief Get the All Hosts from the list
   *
   * @return Q{ist<QPair<QString, QString>>
   */
  QList<QPair<QString, QString>> getAllHosts() {
    // create lambda for map
    auto mapper = [&](auto host) {
      return QPair<QString, QString>(host->getHostname(), host->getIp());
    };

    // create lambda for get
    auto getter = [&](auto item) {
      return qobject_cast<components::Host*>(this->itemWidget(item));
    };

    // create a list
    QList<QPair<QString, QString>> list;

    // iterate through the items
    for (int i = 0; i < this->count(); i++) {
      // get the host from the item
      auto host = getter(this->item(i));

      // check the conditions
      if (host == nullptr) { continue; }

      // add the host to the list
      list.append(mapper(host));
    }

    return list;  // return the list of host
  }

  /**
   * @brief Remove a Host from the list
   * @param hostName
   * @param ip
   */
  void removeHost(QString hostName, QString ip) {
    // create lambda for find
    auto matcher = [&](auto host) {
      return host->getHostname() == hostName && host->getIp() == ip;
    };

    // create lambda for get
    auto getter = [&](auto item) {
      return qobject_cast<components::Host*>(this->itemWidget(item));
    };

    // iterate through the items
    for (int i = 0; i < this->count(); i++) {
      // get the host from the item
      auto host = getter(this->item(i));

      // check the conditions
      if (host == nullptr || !matcher(host)) {
        continue;
      }

      // remove the item
      this->removeItemWidget(this->item(i));
    }
  }

  /**
   * @brief Remove all Hosts from the list
   */
  void removeAllHosts() {
    for (int i = 0; i < this->count(); i++) {
      this->removeItemWidget(this->item(i));
    }
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
