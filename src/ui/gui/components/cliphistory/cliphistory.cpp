#include "cliphistory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Clip Hist object
 *
 * @param parent
 */
ClipHistory::ClipHistory(QWidget *parent) : QWidget(parent) {
  // set alignment from start and center
  clipListLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

  // set 0 margin
  clipListLayout->setContentsMargins(0, 0, 0, 0);

  // set alignment as center
  label->setAlignment(Qt::AlignCenter);

  // set the style sheet
  label->setStyleSheet("QLabel { color : gray; }");

  // add the layout to the stack layout
  this->stackLayout->addWidget(label);

  // set the alignment
  this->stackLayout->setAlignment(label, Qt::AlignCenter);

  // add the layout to the stack layout
  this->stackLayout->addWidget(clipListWidget);

  // set the layout
  this->setLayout(this->stackLayout);

  // set object name
  this->setObjectName("ClipHistory");

  // set up initial language
  this->setUpLanguage();

  // create max clip tile
  for (auto i = 0; i < constants::getAppMaxHistorySize(); i++) {
    this->list.append(new ClipTile());
  }
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void ClipHistory::setUpLanguage() {
  this->label->setText(QObject::tr("No History"));
}

/**
 * @brief on clipDelete Impl
 */
void ClipHistory::onClipDeleteImpl(int idx) {
  auto tile = this->list.at(idx);
  clipListLayout->removeWidget(tile);
  tile->setVisible(false);
  tile->setClip(QVector<QPair<QString, QByteArray>>());
  tile->setVisible(false);
  tile->disconnect();
  tile->setParent(nullptr);

  this->repaint();

  QMetaObject::invokeMethod(
    this, "onClipDelete", Qt::QueuedConnection, Q_ARG(int, idx)
  );
}

/**
 * @brief on clipCopy Impl
 */
void ClipHistory::onClipCopyImpl(int idx) {
  emit onClipSelected(idx);
}

/**
 * @brief Set the History
 */
void ClipHistory::setHistory(const QList<QVector<QPair<QString, QByteArray>>> &history) {
  // clear the layout
  QLayoutItem* item;
  while ((item = clipListLayout->takeAt(0)) != nullptr) {
    auto tile = (ClipTile*) item->widget();
    tile->setClip(QVector<QPair<QString, QByteArray>>());
    tile->setVisible(false);
    tile->disconnect();
    tile->setParent(nullptr);
    delete item;
  }

  // set history
  this->history = history;

  // set the history
  for (auto idx = 0L; idx < history.size(); idx++) {
    // get the tile from the list
    ClipTile *tile  = this->list.at(idx);

    // set the clip
    tile->setClip(history.at(idx));

    // connect the copy signal to this signal
    QObject::connect(
      tile, &components::ClipTile::onClipDelete,
      [=]() { this->onClipDeleteImpl(idx); }
    );

    // connect the select signal to this signal
    QObject::connect(
      tile, &components::ClipTile::onClipCopy,
      [=]() { this->onClipCopyImpl(idx); }
    );

    // add the item to the layout
    clipListLayout->addWidget(tile);

    // set the visibility
    tile->setVisible(true);
  }

  // update
  this->repaint();
}

/**
 * @brief Clear the History
 */
void ClipHistory::clearHistory() {
  // clear the layout
  QLayoutItem* item;
  while ((item = clipListLayout->takeAt(0)) != nullptr) {
    auto tile = (ClipTile*) item->widget();
    tile->setClip(QVector<QPair<QString, QByteArray>>());
    tile->setVisible(false);
    tile->disconnect();
    tile->setParent(nullptr);
    delete item;
  }

  // update
  this->repaint();

  // clear the history
  this->history.clear();
}

/**
 * @brief get the History
 */
QList<QVector<QPair<QString, QByteArray>>> ClipHistory::getHistory() {
  return history;
}

/**
 * @brief Destroy the Clip Hist object
 */
ClipHistory::~ClipHistory() {
  // clear the layout
  QLayoutItem* item;
  while ((item = clipListLayout->takeAt(0)) != nullptr) {
    auto tile = (ClipTile*) item->widget();
    tile->disconnect();
    tile->setClip(QVector<QPair<QString, QByteArray>>());
    tile->setVisible(false);
    tile->setParent(nullptr);
    delete item;
  }

  // delete clipHist
  for (auto clipHist: this->list) {
    clipHist->deleteLater();
  }
}

/**
 * @brief change event
 */
void ClipHistory::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}

/**
 * @brief Paint event
 */
void ClipHistory::paintEvent(QPaintEvent *event) {
  // if the vertical layout is empty then add a label
  if (clipListLayout->count() == 0) {
    this->stackLayout->setCurrentIndex(0);
  } else {
    this->stackLayout->setCurrentIndex(1);
  }

  // For Style sheet
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
