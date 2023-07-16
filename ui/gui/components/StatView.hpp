#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>

#include "ui/gui/components/core/Circle.hpp"
#include "ui/gui/components/core/Label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class StatView : public QWidget {
 public:   // enum for this class
  enum class Value { Connected, Disconnected, Active, Inactive };

 private:  // Constants for style
  static constexpr const char* const style = R"(
  )";

 private:  // disable copy and move for this class
  Q_DISABLE_COPY_MOVE(StatView)

 private:  // just for Qt
  Q_OBJECT

 private:  // Member variable
  core::Circle* circle = new core::Circle(this);
  core::Label* status = new core::Label(this);

 private:  // Member variable
  Value value = Value::Disconnected;

 public:
  /**
   * @brief Construct a new StatView object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit StatView(QWidget* parent = nullptr) : QWidget(parent) {
    // create a layout to align the widgets
    auto root = new QHBoxLayout(this);

    // add the widgets to the layout
    root->addWidget(circle);
    root->addWidget(status);

    // set the layout to the widget
    this->setLayout(root);

    // set the Style to the widget
    this->setStyleSheet(style);
  }

  /**
   * @brief Destroy the StatView object
   */
  virtual ~StatView() = default;

  /**
   * @brief set the StatView
   * @param val StatView
   */
  void setStatus(Value val) {
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
  }

  /**
   * @brief get the StatView
   * @return QString value
   */
  Value getStatus() const { return value; }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
