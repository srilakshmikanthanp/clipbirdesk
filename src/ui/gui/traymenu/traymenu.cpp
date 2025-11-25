#include "traymenu.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Construct a new Sys Tray object
 *
 * @param parent
 */
TrayMenu::TrayMenu(QWidget* parent) : QMenu(parent) {
  // connect signals
  QObject::connect(&exitApp, &QAction::triggered, this, &TrayMenu::OnExitClicked);
  QObject::connect(&openApp, &QAction::triggered, this, &TrayMenu::OnOpenAppClicked);

  this->addAction(&openApp);
  this->addSeparator();
  this->addAction(&exitApp);

  // set up the initial language
  this->setUpLanguage();
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void TrayMenu::setUpLanguage() {
  openApp.setText(QObject::tr("Open Clipbird"));
  exitApp.setText(QObject::tr("Exit"));
}

/**
 * @brief set Exit Enabled or Disabled
 */
void TrayMenu::setExitEnabled(bool isenabled) {
  exitApp.setEnabled(isenabled);
}

/**
 * @brief set open app Enabled or Disabled
 */
void TrayMenu::setOpenAppEnabled(bool isenabled) {
  openApp.setEnabled(isenabled);
}

/**
 * @brief Is Exit Enabled
 */
bool TrayMenu::isExitEnabled() const {
  return exitApp.isEnabled();
}

/**
 * @brief Is Open App Enabled
 */
bool TrayMenu::isOpenAppEnabled() const {
  return openApp.isEnabled();
}

/**
 * @brief change event
 */
void TrayMenu::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}

/**
 * @brief Override paint event
 */
void TrayMenu::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QPainterPath path;
  QRectF rect = QRectF(this->rect()).adjusted(2.0, 2.0, -2.0, -2.0);
  path.addRoundedRect(rect, 7, 7);
  QRegion region = QRegion(path.toFillPolygon().toPolygon());
  this->setMask(region);

  QMenu::paintEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
