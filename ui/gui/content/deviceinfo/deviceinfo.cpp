// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "deviceinfo.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::content {
/**
 * @brief Construct a new Device Info:: Device Info object
 *
 * @param parent
 */
DeviceInfo::DeviceInfo(QWidget* parent) : QWidget(parent) {
  // create all the components
  this->hostStatus.first  = new QLabel();
  this->hostStatus.second = new components::Status();

  this->groupName.first  = new QLabel();
  this->groupName.second = new QLabel();

  this->hostCount.first   = new QLabel();
  this->hostCount.second  = new QLabel();

  // set Alignment
  this->hostStatus.first->setAlignment(Qt::AlignLeft);
  this->hostStatus.second->setAlignment(Qt::AlignRight);

  this->groupName.first->setAlignment(Qt::AlignLeft);
  this->groupName.second->setAlignment(Qt::AlignRight);

  this->hostCount.first->setAlignment(Qt::AlignLeft);
  this->hostCount.second->setAlignment(Qt::AlignRight);

  // create the top layout
  auto top = new QFormLayout();

  // add top layout components
  top->addRow(this->groupName.first, this->groupName.second);
  top->addRow(this->hostStatus.first, this->hostStatus.second);
  top->addRow(this->hostCount.first, this->hostCount.second);

  // set top layout alignment
  top->setLabelAlignment(Qt::AlignLeft);
  top->setFormAlignment(Qt::AlignRight);

  // set top layout to this
  this->setLayout(top);
}

/**
 * @brief Set the Host Label and Status
 *
 * @param QPair
 */
void DeviceInfo::setHostStatus(const QPair<QString, components::Status::Value>& host) {
  this->hostStatus.first->setText(host.first);
  this->hostStatus.second->setStatus(host.second);
}

/**
 * @brief Set the Server Name
 *
 * @param QPair
 */
void DeviceInfo::setGroupName(const QPair<QString, QString>& name) {
  this->groupName.first->setText(name.first);
  this->groupName.second->setText(name.second);
}

/**
 * @brief Set the Host Count
 *
 * @param QPair
 */
void DeviceInfo::setHostCount(const QPair<QString, QString>& count) {
  this->hostCount.first->setText(count.first);
  this->hostCount.second->setText(count.second);
}

/**
 * @brief Get the Host Label and Status
 *
 * @return QPair
 */
QPair<QString, components::Status::Value> DeviceInfo::getHostStatus() const {
  return {this->hostStatus.first->text(), this->hostStatus.second->getStatus()};
}

/**
 * @brief Get the Server Name
 *
 * @return QPair
 */
QPair<QString, QString> DeviceInfo::getGroupName() const {
  return {this->groupName.first->text(), this->groupName.second->text()};
}

/**
 * @brief Get the Host Count
 *
 * @return QPair
 */
QPair<QString, QString> DeviceInfo::getHostCount() const {
  return {this->hostCount.first->text(), this->hostCount.second->text()};
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
