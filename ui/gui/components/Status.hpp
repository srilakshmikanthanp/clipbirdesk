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
 private:  // Member variable
  individual::Circle *circle;
  individual::Label *status;

 private:  // just for Qt
  Q_OBJECT

 private: // disable copy and move
  Q_DISABLE_COPY_MOVE(Status)

 public:
  /**
   * @brief Construct a new Status object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Status(QWidget* parent = nullptr) {
    // create the components of the class
    this->circle = new individual::Circle(this);
    this->status = new individual::Label(this);

    // create a layout to align the widgets
    auto root = new QHBoxLayout(this);

    // add the widgets to the layout
    root->addWidget(this->circle);
    root->addWidget(this->status);

    // set the layout to the widget
    this->setLayout(root);
  }

  /**
   * @brief Destroy the Status object
   */
  virtual ~Status() = default;

  /**
   * @brief set the color
   * @param color color
   */
  void setColor(const QColor& color) {
    this->circle->setColor(color);
  }

  /**
   * @brief set the Status
   * @param val Status
   */
  void setStatus(const QString& val) {
    this->status->setText(val);
  }

  /**
   * @brief get the color
   * @return QColor color
   */
  QColor getColor() const {
    return this->circle->getColor();
  }

  /**
   * @brief get the Status
   * @return QString value
   */
  QString getStatus() const {
    return this->status->text();
  }
};
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
