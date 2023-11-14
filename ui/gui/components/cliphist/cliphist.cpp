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
  this->setStyleSheet(R"(
    border-bottom-left-radius: 10px;
    border-bottom-right-radius: 10px;
    color: white;
    background: #2d2d2d;
    border-top-left-radius: 10px;
    border-top-right-radius: 10px;
  )");

  // no spacing between label and buttons
  layout->setSpacing(0);

  // set properties
  item->setAlignment(Qt::AlignLeft);
  item->setMargin(10);
  item->setWordWrap(true);
  item->setFixedHeight(95);

  // delete button for clip
  auto del = new QPushButton();
  auto cpy = new QPushButton();

  // set icons
  del->setIcon(QIcon(":/images/delete.png"));
  cpy->setIcon(QIcon(":/images/copy.png"));

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

  // set fixed size for tail
  this->setFixedSize(360, 150);

  // infer the data
  for (const auto &[mime, data] : clip) {
    // has Image png
    if (mime == MIME_TYPE_PNG) {
      auto pixmap = QPixmap::fromImage(QImage::fromData(data));
      auto icon   = QIcon();
      icon.addPixmap(pixmap);
      item->setPixmap(icon.pixmap(QSize(512, 512)));
      break;
    }

    // has HTML
    if (mime == MIME_TYPE_HTML) {
      item->setText(QString::fromUtf8(data));
      break;
    }

    // has Text
    if (mime == MIME_TYPE_TEXT) {
      item->setText(QString::fromUtf8(data));
      break;
    }
  }
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
  QLabel *label = new QLabel("No Hosts");

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
}

/**
 * @brief Set the History
 */
void ClipHist::setHistory(const QList<QVector<QPair<QString, QByteArray>>> &history) {
  // set the history
  this->history = history;

  // clear the layout
  this->clearHistory();

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

  // Redraw the widget
  this->repaint();

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

  // clear the history
  this->history.clear();

  // Redraw the widget
  this->repaint();

  // update
  this->update();
}

/**
 * @brief Get the History
 */
const QList<QVector<QPair<QString, QByteArray>>> &ClipHist::getHistory() const {
  return history;
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

  // call the base class
  QWidget::paintEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
