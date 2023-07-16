#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>

#include "ui/gui/components/individual/Circle.hpp"
#include "ui/gui/components/individual/Label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Status : public QWidget {
 private:  // disable copy and move for this class
  Q_DISABLE_COPY_MOVE(Status)

 public:   // enum for this class
  enum class Value { Connected, Disconnected, Active, Inactive };

 private:  // Member variable
  Value value = Value::Disconnected;

 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Status object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Status(QWidget* parent = nullptr) : QWidget(parent) {}

  /**
   * @brief Destroy the Status object
   */
  virtual ~Status() = default;

  /**
   * @brief set the Status
   * @param val Status
   */
  void setStatus(Value val) {
    // create the components of the class
    auto circle = new individual::Circle(this);
    auto status = new individual::Label(this);

    // create a layout to align the widgets
    auto root = new QHBoxLayout(this);

    // add the widgets to the layout
    root->addWidget(circle);
    root->addWidget(status);

    // set the text of the status
    switch (val) {
      case Value::Disconnected:
        status->setText("Disconnected");
        circle->setColor(Qt::red);
        break;
      case Value::Connected:
        status->setText("Connected");
        circle->setColor(Qt::green);
        break;
      case Value::Active:
        status->setText("Active");
        circle->setColor(Qt::green);
        break;
      case Value::Inactive:
        status->setText("Inactive");
        circle->setColor(Qt::red);
        break;
    }

    // set the layout to the widget
    this->setLayout(root);
  }

  /**
   * @brief get the Status
   * @return QString value
   */
  Value getStatus() const { return value; }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
