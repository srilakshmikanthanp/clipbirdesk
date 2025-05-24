#include "history.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::widgets {
/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
History::History(QWidget * parent) : QWidget(parent) {
  auto scrollArea = new QScrollArea();
  scrollArea->setAlignment(Qt::AlignTop);
  scrollArea->setWidgetResizable(true);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setWidget(clipHist);

  // always set scroll bar at top
  QObject::connect(
    scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged,
    this, [scrollArea]() {
      scrollArea->verticalScrollBar()->setValue(0);
    }
  );

  // create layout VBox
  auto vBox = new QVBoxLayout();
  vBox->setAlignment(Qt::AlignTop);
  vBox->addWidget(this->clipSend);
  vBox->addWidget(scrollArea);

  this->setLayout(vBox);
  this->setUpLanguage();

  // connect the signals
  QObject::connect(
    this->clipHist, &components::ClipHistory::onClipSelected,
    this, &History::onClipSelected
  );

  // connect the signals
  QObject::connect(
    this->clipHist, &components::ClipHistory::onClipDelete,
    this, &History::onClipDelete
  );

  // connect the send signal to this signal
  QObject::connect(
    this->clipSend, &components::ClipSend::onClipSend,
    [this]() { emit onClipSend(); }
  );
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
