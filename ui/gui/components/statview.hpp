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
  explicit StatView(QWidget* parent = nullptr);

  /**
   * @brief Destroy the StatView object
   */
  virtual ~StatView() = default;

  /**
   * @brief set the StatView
   * @param val StatView
   */
  void setStatus(Value val);

  /**
   * @brief get the StatView
   * @return QString value
   */
  Value getStatus() const;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
