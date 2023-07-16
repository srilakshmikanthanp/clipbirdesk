#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFormLayout>
#include <QWidget>

#include "ui/gui/components/individual/Label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Item : public QWidget {
 private:  // Member variable
  QWidget *key;
  QWidget *val;

 private:  // just for Qt
  Q_OBJECT

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(Item)

 public:
  /**
   * @brief Construct a new Item object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Item(QWidget* parent = nullptr) : QWidget(parent) { }

  /**
   * @brief Destroy the Item object
   */
  virtual ~Item() = default;

  /**
   * @brief Set as a pair
   *
   * @param key key
   * @param val value
   */
  void set(QWidget *key, QWidget* val) {
    // create a layout to align the widgets
    auto root = new QFormLayout(this);

    // save the widgets
    this->key = key;
    this->val = val;

    // add the widgets to the layout
    root->addRow(key, val);

    // set the layout to the widget
    this->setLayout(root);
  }

  /**
   * @brief get the key and value as pair
   *
   * @return std::pair<QString, QString> key and value
   */
  QPair<QWidget*, QWidget*> get() const {
    return {this->key, this->val};
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
