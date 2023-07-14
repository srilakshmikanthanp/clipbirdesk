#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core {
class Item : public QWidget {
 private:  // Member variable
  QWidget *label;
  QWidget *value;
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Item object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Item(QWidget* parent = nullptr) : QWidget(parent) {
    // set layout to QBocLayout in LeftToRight direction
    this->setLayout(new QHBoxLayout(this));

    // TODO: set style sheet
  }

  /**
   * @brief set the Key of the Item
   */
  void setLabel(QWidget* label) {
    auto layout = qobject_cast<QBoxLayout*>(this->layout());
    layout->addWidget((this->label = label), 0, Qt::AlignLeft);
  }

  /**
   * @brief set the Value of the Item
   */
  void setValue(QWidget* value) {
    auto layout = qobject_cast<QBoxLayout*>(this->layout());
    layout->addWidget((this->value = value), 0, Qt::AlignRight);
  }

  /**
   * @brief get the Key of the Item
   */
  QWidget* getLabel() {
    return label;
  }

  /**
   * @brief get the Value of the Item
   */
  QWidget* getValue() {
    return value;
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
