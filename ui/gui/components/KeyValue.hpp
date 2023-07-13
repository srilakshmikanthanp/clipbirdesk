#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHboxLayout>
#include <QWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core {
class KeyValue : public QWidget {
 private:  // Member variable
    QWidget *key;
    QWidget *value;
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new KeyValue object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit KeyValue(QWidget* parent = nullptr) : QWidget(parent) {
    this->setLayout(new QHboxLayout(this));
    // TODO: set style sheet
  }

  /**
   * @brief set the Key of the KeyValue
   */
  void setKey(QWidget* key) {
    auto layout = static_cast<QHboxLayout*>(this->layout());
    layout->addWidget((this->key = key), 0, Qt::AlignLeft);
  }

  /**
   * @brief set the Value of the KeyValue
   */
  void setValue(QWidget* value) {
    auto layout = static_cast<QHboxLayout*>(this->layout());
    layout->addWidget((this->value = value), 0, Qt::AlignRight);
  }

  /**
   * @brief get the Key of the KeyValue
   */
  QWidget& getKey() {
    return key;
  }

  /**
   * @brief get the Value of the KeyValue
   */
  QWidget& getValue() {
    return value;
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
