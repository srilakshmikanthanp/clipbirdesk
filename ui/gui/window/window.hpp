// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QGuiApplication>
#include <QMainWindow>
#include <QScreen>
#include <QVBoxLayout>

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class Window : public QMainWindow {
 private:  // Member variable

  QSize ratio = QSize(3, 3);

 private:  // Disable copy, move and assignment

  Q_DISABLE_COPY_MOVE(Window);

 public:  // Constructor and Destructor

  /**
   * @brief Construct a new Gui Main object
   *
   * @param parent parent widget
   */
  explicit Window(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Gui Main object
   *
   */
  ~Window() noexcept = default;

 protected:  // protected functions

  /**
   * @brief Override the setVisible function
   */
  void setVisible(bool visible) override;

  /**
   * @brief On Show Event
   */
  void showEvent(QShowEvent* event) override;

  /**
   * @brief Focus lost event
   */
  bool event(QEvent* event) override;
};  // class Window
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
