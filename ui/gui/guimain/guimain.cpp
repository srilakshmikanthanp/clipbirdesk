// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "guimain.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Construct a new Gui Main object
 */
GuiMain::GuiMain(QWidget* parent) : QMainWindow(parent) {
  // set mainWindow attributes
  this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  // set as Translucent background
  this->setAttribute(Qt::WA_TranslucentBackground);

  // set padding as 20px
  this->setContentsMargins(20, 20, 20, 20);

  // set the object name
  this->setObjectName("GuiMain");
}

/**
 * @brief Set the Size Ratio object
 */
void GuiMain::setSizeRatio(QSize r) {
  this->ratio = r;
}

/**
 * @brief Get the Size Ratio object
 */
QSize GuiMain::getSizeRatio() {
  return this->ratio;
}

/**
 * @brief Override the setVisiblity
 */
void GuiMain::setVisible(bool visible) {
  // if visible
  if (!visible) return QWidget::setVisible(visible);

  // Get the Primary Screen
  auto screen  = QGuiApplication::primaryScreen();

  // get the screen size
  auto sc_siz  = screen->availableGeometry();

  // Calculate the GuiMain size from ratio
  auto win_siz = QSize();

  // calc the Height
  win_siz.setHeight(sc_siz.height() / ratio.height());

  // calc the Width
  win_siz.setWidth(sc_siz.width() / ratio.width());

  // set the GuiMain size
  setFixedSize(win_siz);

  // move the GuiMain to center
  move(screen->geometry().center() - rect().center());

  // show the GuiMain
  QWidget::setVisible(visible);
}

/**
 * @brief On Show Event
 */
void GuiMain::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  QWidget::setFocus();
  QWidget::activateWindow();
  QWidget::raise();
}

/**
 * @brief Override the focusOutEvent
 *
 * @param event
 */
bool GuiMain::event(QEvent* event) {
  // if event is window deactivation
  if (event->type() == QEvent::WindowDeactivate) {
    this->hide();
  }

  // call the base class event
  return QWidget::event(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
