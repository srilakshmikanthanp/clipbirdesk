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

  // create layout VBox
  auto vBox = new QVBoxLayout();

  // add scroll area to layout
  vBox->addWidget(historyArea);

  // set layout to widget
  this->setLayout(vBox);
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
}
