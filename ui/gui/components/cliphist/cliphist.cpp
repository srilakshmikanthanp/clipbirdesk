// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "cliphist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Clip Tile object
 *
 * @param parent
 */
ClipTile::ClipTile(QWidget *parent): QWidget(parent) {
  // vertical layout for adding Label and buttons
  auto layout = new QVBoxLayout();

  // set the style
  this->setObjectName("ClipTile");

  // no spacing between label and buttons
  layout->setSpacing(0);

  // set properties
  item->setAlignment(Qt::AlignLeft);

  // delete button for clip
  auto del = new QPushButton();
  auto cpy = new QPushButton();

  // Connect Signals
  QObject::connect(del, &QPushButton::clicked, [this]() { emit onClipDelete(); });
  QObject::connect(cpy, &QPushButton::clicked, [this]() { emit onClipCopy(); });

  // set icons
  del->setIcon(QIcon(":/images/delete.png"));
  cpy->setIcon(QIcon(":/images/copy.png"));

  // pointer
  del->setCursor(Qt::PointingHandCursor);
  cpy->setCursor(Qt::PointingHandCursor);

  // hBox to put copy and delete buttons
  auto hBox = new QHBoxLayout();

  // set Alignment
  hBox->setAlignment(Qt::AlignRight);
  hBox->addWidget(cpy);
  hBox->addWidget(del);

  // add Label and button to main vertical layout
  layout->addWidget(item);
  layout->addLayout(hBox);

  // add main layout to widget
  this->setLayout(layout);

  // set up linguist
  this->setUpLanguage();
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void ClipTile::setUpLanguage() {
  // No thing to Do
}

/**
 * @brief set the clip
 */
void ClipTile::setClip(const QVector<QPair<QString, QByteArray>> &clip) {
  // infer the data
  for (const auto &[mime, data] : clip) {
    // has Image png ans size is less than 1mb
    if (mime == MIME_TYPE_PNG && data.size() > IMG_SIZE) {
      auto icon = QPixmap::fromImage(QImage(":/images/photo.png"));
      item->setPixmap(icon.scaled(30, 30, Qt::KeepAspectRatio));
      break;
    } else if (mime == MIME_TYPE_PNG) {
      auto icon = QPixmap::fromImage(QImage::fromData(data));
      item->setPixmap(icon.scaled(100, 100, Qt::KeepAspectRatio));
      break;
    }

    // has Text trim the text to 100 characters
    if (mime == MIME_TYPE_TEXT) {
      item->setText(QString::fromUtf8(data).left(TXT_SIZE));
      break;
    }
  }

  this->clip = clip;
}

/**
 * @brief Clear the clip
 */
QVector<QPair<QString, QByteArray>> ClipTile::getClip() {
  return this->clip;
}

/**
 * @brief Paint event
 */
void ClipTile::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/**
 * @brief change event
 */
void ClipTile::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}

/**
 * @brief Construct a new Clip Hist object
 *
 * @param parent
 */
ClipHist::ClipHist(QWidget *parent) : QWidget(parent) {
  // set alignment from start and center
  verticalLayout->setAlignment(Qt::AlignTop);

  // set alignment as center
  label->setAlignment(Qt::AlignCenter);

  // set the style sheet
  label->setStyleSheet("QLabel { color : gray; }");

  // add the layout to the stack layout
  this->stackLayout->addWidget(label);

  // set the alignment
  this->stackLayout->setAlignment(label, Qt::AlignCenter);

  // add the layout to the stack layout
  this->stackLayout->addWidget(verticalWidget);

  // set the layout
  this->setLayout(this->stackLayout);

  // set object name
  this->setObjectName("ClipHist");

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
void ClipHist::setUpLanguage() {
  this->label->setText(QObject::tr("Nothing so far"));
}

/**
 * @brief on clipDelete Impl
 */
void ClipHist::onClipDeleteImpl(int idx) {
  emit onClipDelete(idx);
}

/**
 * @brief on clipCopy Impl
 */
void ClipHist::onClipCopyImpl(int idx) {
  emit onClipSelected(idx);
}

/**
 * @brief Set the History
 */
void ClipHist::setHistory(const QList<QVector<QPair<QString, QByteArray>>> &history) {
  // clear the layout
  QLayoutItem* item;
  while ((item = verticalLayout->takeAt(0)) != nullptr) {
    auto tile = (ClipTile*) item->widget();
    tile->setClip(QVector<QPair<QString, QByteArray>>());
    tile->disconnect();
    tile->setVisible(false);
    tile->setParent(nullptr);
    delete item;
  }

  // set history
  this->history = history;

  // set the history
  for (auto idx = 0L; idx < history.size(); idx++) {
    // get the tile from the list
    ClipTile *tile  = this->list.at(idx);

    // connect the copy signal to this signal
    auto signal_d = &components::ClipTile::onClipDelete;
    auto slot_d   = [=]() { this->onClipDeleteImpl(idx); };
    QObject::connect(tile, signal_d, slot_d);

    // connect the select signal to this signal
    auto signal_c = &components::ClipTile::onClipCopy;
    auto slot_c   = [=]() { this->onClipCopyImpl(idx); };
    QObject::connect(tile, signal_c, slot_c);

    // set the clip
    tile->setClip(history.at(idx));

    // add the item to the layout
    verticalLayout->addWidget(tile);

    // set the visibility
    tile->setVisible(true);
  }

  // update
  this->repaint();
}

/**
 * @brief Clear the History
 */
void ClipHist::clearHistory() {
  // clear the layout
  QLayoutItem* item;
  while ((item = verticalLayout->takeAt(0)) != nullptr) {
    auto tile = (ClipTile*) item->widget();
    tile->disconnect();
    tile->setClip(QVector<QPair<QString, QByteArray>>());
    tile->setVisible(false);
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
QList<QVector<QPair<QString, QByteArray>>> ClipHist::getHistory() {
  return history;
}

/**
 * @brief Destroy the Clip Hist object
 */
ClipHist::~ClipHist() {
  // clear the layout
  QLayoutItem* item;
  while ((item = verticalLayout->takeAt(0)) != nullptr) {
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
void ClipHist::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}

/**
 * @brief Paint event
 */
void ClipHist::paintEvent(QPaintEvent *event) {
  // if the vertical layout is empty then add a label
  if (verticalLayout->count() == 0) {
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
