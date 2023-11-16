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
ClipTile::ClipTile(const QVector<QPair<QString, QByteArray>> &clip, QWidget *parent): QWidget(parent) {
  // vertical layout for adding Label and buttons
  auto layout = new QVBoxLayout();

  // set the style
  this->setObjectName("ClipTile");

  // no spacing between label and buttons
  layout->setSpacing(0);

  // create a label
  auto item = new QLineEdit(this);

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

  // fixed width
  this->setFixedWidth(300);

  // infer the data
  for (const auto &[mime, data] : clip) {
    // has Image png
    if (mime == MIME_TYPE_PNG) {
      // auto icon   = QIcon(QPixmap::fromImage(QImage::fromData(data)));
      // item->setPixmap(icon.pixmap(QSize(512, 512)));
      // break;
    }

    // has Text
    if (mime == MIME_TYPE_TEXT) {
      item->setText(QString::fromUtf8(data));
      break;
    }
  }
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
 * @brief Construct a new Clip Hist object
 *
 * @param parent
 */
ClipHist::ClipHist(QWidget *parent) : QWidget(parent) {
  // set alignment from start and center
  verticalLayout->setAlignment(Qt::AlignTop);

  // create a label
  QLabel *label = new QLabel("Nothing so far");

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
}

/**
 * @brief Set the History
 */
void ClipHist::setHistory(const QList<QVector<QPair<QString, QByteArray>>> &history) {
  // clear the layout without Repaint
  QLayoutItem* item;
  while ((item = verticalLayout->takeAt(0)) != nullptr) {
    verticalLayout->removeItem(item);
    delete item->widget();
    delete item;
  }

  // set the history
  for (auto idx = 0L; idx < history.size(); idx++) {
    // create a new host view
    ClipTile *tile  = new ClipTile(history.at(idx));

    // connect the host view signal to this signal
    auto signal_d = &components::ClipTile::onClipDelete;
    auto slot_d   = [=]() { emit onClipDelete(idx); };
    QObject::connect(tile, signal_d, slot_d);

    auto signal_c = &components::ClipTile::onClipCopy;
    auto slot_c   = [=]() { emit onClipSelected(idx); };
    QObject::connect(tile, signal_c, slot_c);

    // add the host view to the layout
    verticalLayout->addWidget(tile);
  }

  // update
  this->update();
}

/**
 * @brief Clear the History
 */
void ClipHist::clearHistory() {
  // clear the layout
  QLayoutItem* item;
  while ((item = verticalLayout->takeAt(0)) != nullptr) {
    verticalLayout->removeItem(item);
    delete item->widget();
    delete item;
  }

  // update
  this->update();
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
