#include "clipsend.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Clip Tile object
 *
 * @param parent
 */
ClipSend::ClipSend(QWidget *parent): QWidget(parent) {
  // Horizontal layout for adding Label and butto
  auto layout = new QHBoxLayout();

  this->setObjectName("ClipSend");

  // label
  auto item = new QLabel("Send clipboard to other devices");

  // button
  auto send = new QPushButton("Send");

  // Connect Signals
  QObject::connect(send, &QPushButton::clicked, [this]() { emit onClipSend(); });

  // pointer
  send->setCursor(Qt::PointingHandCursor);

  layout->addWidget(item, Qt::AlignLeft);
  layout->addWidget(send, Qt::AlignRight);

  // add main layout to widget
  this->setLayout(layout);

  // set up linguist
  this->setUpLanguage();
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void ClipSend::setUpLanguage() {
  // No thing to Do
}

/**
 * @brief Paint event
 */
void ClipSend::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/**
 * @brief change event
 */
void ClipSend::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
