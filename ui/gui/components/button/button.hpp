#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QPushButton>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Button : public QPushButton {
 private:  // Constants

  static constexpr const char *const style = R"(
    // Base Style for Button
    QPushButton {
      background-color: #2b2a3c;
      border: 1px solid #2b2a3c;
      border-radius: 10px;
      color: #ffffff;
      font-size: 10px;
    }

    QPushButton#Connect {
      padding-left: 11px;
      padding-right: 11px;
      padding-top: 5px;
      padding-bottom: 5px;
    }

    QPushButton#Connect:hover {
      background-color: #0375ef;
      border: 1px solid #0375ef;
    }

    QPushButton#Disconnect {
      padding: 5px;
    }
    
    QPushButton#Disconnect:hover {
      background-color: #e06666;
      border: 1px solid #e06666;
    }
  )";

 private:  // just for Qt

  Q_OBJECT

 public:

  /**
   * @brief Construct a new Button object
   * with parent as QButton
   * @param parent parent object
   */
  explicit Button(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Button object
   * with virtual destructor
   */
  ~Button() override = default;

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Button)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
