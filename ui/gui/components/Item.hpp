#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QWidget>

#include "ui/gui/components/individual/Label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Item : public QWidget {
 private:  // Member variable
  individual::Label *key;
  individual::Label *val;

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
  explicit Item(QWidget* parent = nullptr) {
    // create the components of the class
    this->key = new individual::Label(this);
    this->val = new individual::Label(this);

    // create a layout to align the widgets
    auto root = new QHBoxLayout(this);

    // add the widgets to the layout
    root->addWidget(this->key, 0, Qt::AlignLeft);
    root->addWidget(this->val, 0, Qt::AlignRight);

    // set the layout to the widget
    this->setLayout(root);

    // TODO: set the style sheet
  }

  /**
   * @brief Destroy the Item object
   */
  virtual ~Item() = default;

  /**
   * @brief set the key
   * @param key key
   */
  void setKey(const QString& key) {
    this->key->setText(key);
  }

  /**
   * @brief set the value
   * @param val value
   */
  void setVal(const QString& val) {
    this->val->setText(val);
  }

  /**
   * @brief Set as a pair
   *
   * @param key key
   * @param val value
   */
  void set(const QString& key, const QString& val) {
    this->key->setText(key);
    this->val->setText(val);
  }

  /**
   * @brief get the key
   * @return QString key
   */
  QString getKey() const {
    return this->key->text();
  }

  /**
   * @brief get the value
   * @return QString value
   */
  QString getVal() const {
    return this->val->text();
  }

  /**
   * @brief get the key and value as pair
   *
   * @return std::pair<QString, QString> key and value
   */
  std::pair<QString, QString> get() const {
    return {this->key->text(), this->val->text()};
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
