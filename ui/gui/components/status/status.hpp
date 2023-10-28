#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Status : public QLabel {
 public:  // enum for this class

  enum class Value { Connected, Disconnected, Active, Inactive };

 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Status)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  Value value = Value::Disconnected;

 public:

  /**
   * @brief Construct a new Status object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Status(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Status() = default;

  /**
   * @brief set the Status
   * @param val Status
   */
  void setStatus(Value val);

  /**
   * @brief get the Status
   * @return QString value
   */
  Value getStatus() const;

  /**
   * @brief Set Both at one time
   */
  void set(const QString& text, Value val);

  /**
   * @brief Get Both at one time
   */
  QPair<QString, Value> get() const;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
