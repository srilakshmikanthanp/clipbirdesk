#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QGuiApplication>
#include <QGraphicsDropShadowEffect>
#include <QMainWindow>
#include <QScreen>
#include <QVBoxLayout>

#include "ui/gui/screen/clipbird/clipbird.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class Container : public QWidget {
 private:  // Member variable

  ui::gui::Clipbird* content = nullptr;
  QSize ratio = QSize(3, 3);

 private:  // Disable copy, move and assignment

  Q_DISABLE_COPY_MOVE(Container);

 public:  // Constructor and Destructor

  /**
   * @brief Construct a new Gui Main object
   *
   * @param parent parent widget
   */
  explicit Container(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Gui Main object
   *
   */
  ~Container() noexcept = default;

  /**
   * @brief Set Content
   */
  void setContent(ui::gui::Clipbird* content);

  /**
   * @brief get Content
   */
  ui::gui::Clipbird* getContent() const;

 protected:  // protected functions

  /**
   * @brief Override the setVisible function
   */
  void setVisible(bool visible) override;

  /**
   * @brief Focus lost event
   */
  bool event(QEvent* event) override;

  /**
   * @brief On Show Event
   */
  void showEvent(QShowEvent* event) override;
};  // class Container
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
