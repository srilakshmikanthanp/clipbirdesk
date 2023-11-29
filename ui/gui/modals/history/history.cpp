// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "history.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
History::History(QWidget * parent) : QDialog(parent) {
  // Create QScrollArea
  auto historyArea = new QScrollArea();

  // set the widget as Resizable
  historyArea->setWidgetResizable(true);

  // set the widget to scroll area
  historyArea->setWidget(this->clipHist);

  // Align center the List
  historyArea->setAlignment(Qt::AlignCenter);

  // always set scroll bar at top
  const auto signal = &QScrollBar::rangeChanged;
  const auto slot   = [historyArea]() { historyArea->verticalScrollBar()->setValue(0); };
  QObject::connect(historyArea->verticalScrollBar(), signal, this, slot);

  // create layout VBox
  auto vBox = new QVBoxLayout();

  // add scroll area to layout
  vBox->addWidget(historyArea);

  // set layout to widget
  this->setLayout(vBox);

  // set up language
  this->setUpLanguage();

  // connect the signals
  auto signal_s = &components::ClipHist::onClipSelected;
  auto slot_s  = &History::onClipSelected;
  QObject::connect(this->clipHist, signal_s, this, slot_s);

  // connect the signals
  auto signal_d = &components::ClipHist::onClipDelete;
  auto slot_d  = &History::onClipDelete;
  QObject::connect(this->clipHist, signal_d, this, slot_d);
}

/**
 * @brief Set the History
 */
void History::setHistory(const QList<QVector<QPair<QString, QByteArray>>> &hist) {
  this->clipHist->setHistory(hist);
}

/**
 * @brief Clear the History
 */
void History::clearHistory() {
  this->clipHist->clearHistory();
}

/**
 * @brief get the history
 */
QList<QVector<QPair<QString, QByteArray>>> History::getHistory() const {
  return this->clipHist->getHistory();
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void History::setUpLanguage() {
  // Nothing to do
}

/**
 * @brief override set visible
 */
void History::setVisible(bool visible) {
  // call base class setVisible
  QWidget::setVisible(visible);

  // if not visible
  if (!visible) return;

  // get the screen size
  auto screen = QGuiApplication::primaryScreen()->availableGeometry();

  // get the widget size
  auto size = this->size();

  // create a new geometry
  QRect geometry;

  // set the geometry to right bottom with margin 20
  geometry.setHeight(size.height());
  geometry.setY(screen.height() - size.height() - 5);
  geometry.setX(screen.width() - size.width() - 5);
  geometry.setWidth(size.width());

  // set the geometry
  this->setGeometry(geometry);
}


/**
 * @brief change event
 */
void History::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
}
