// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "request.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Construct a new Request:: Request object
 *
 * @param parent
 */
Request::Request(QWidget *parent) : QDialog(parent) {
  // vertical layout for adding Label and buttons
  auto layout = new QVBoxLayout();

  // set the style
  this->setObjectName("Request");

  // spacing between label and buttons
  layout->setSpacing(10);

  // set properties
  prompt->setAlignment(Qt::AlignCenter);
  host->setAlignment(Qt::AlignCenter);

  // font size
  prompt->setStyleSheet("font-size: 25px;");
  host->setStyleSheet("font-size: 14px;");

  // set warp text
  prompt->setWordWrap(true);
  host->setWordWrap(true);

  // pointer
  accept->setCursor(Qt::PointingHandCursor);
  reject->setCursor(Qt::PointingHandCursor);

  // hBox to put copy and delete buttons
  auto hBox = new QHBoxLayout();

  // spacing between buttons
  hBox->setSpacing(10);

  // set Alignment
  hBox->setAlignment(Qt::AlignCenter);
  hBox->addWidget(accept);
  hBox->addWidget(reject);

  // add Label and button to main vertical layout
  layout->addWidget(prompt);
  layout->addWidget(host);
  layout->addLayout(hBox);

  // set padding
  layout->setContentsMargins(20, 20, 20, 20);

  // add main layout to widget
  this->setLayout(layout);

  // set up linguist
  this->setUpLanguage();

  // connect signals
  connect(accept, &QPushButton::clicked, this, &Request::onAccept);
  connect(this, &Request::finished, this, &Request::onReject);
  connect(reject, &QPushButton::clicked, this, &Request::onReject);
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void Request::setUpLanguage() {
  prompt->setText(tr("Do you allow the device to Join your Group"));
  accept->setText(tr("Accept"));
  reject->setText(tr("Reject"));
}

/**
 * @brief set the Host
 *
 * @param text
 */
void Request::setHost(const QString &text) {
  host->setText(text);
}

/**
 * @brief get the Host Text object
 *
 * @return QString
 */
QString Request::getHost() const {
  return host->text();
}

/**
 * @brief override set visible
 */
void Request::setVisible(bool visible) {
  // call base class setVisible
  QWidget::setVisible(visible);

  // if not visible
  if (!visible) return;

  // get the screen size
  auto screen = QGuiApplication::primaryScreen()->availableGeometry();

  // get the widget size
  auto size = this->sizeHint();

  // create a new geometry
  QRect geometry;

  // set the size
  geometry.setSize(size);

  // set the geometry to right bottom with margin 20
  geometry.moveBottomRight(QPoint(screen.bottomRight().x() - 5, screen.bottomRight().y() - 5));

  // set the geometry
  this->setGeometry(geometry);
}

/**
 * @brief Override paint event
 */
void Request::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
}
}
