#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QScrollArea>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Scroll : public QScrollArea {
 private:  // Constants

  static constexpr const char *const style = R"(
  )";

 private:  // just for Qt

  Q_OBJECT

 public:

  /**
   * @brief Construct a new Scroll object
   * with parent as QScrollArea
   * @param parent parent object
   */
  explicit Scroll(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Scroll object
   * with virtual destructor
   */
  ~Scroll() override = default;

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Scroll)
};
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
