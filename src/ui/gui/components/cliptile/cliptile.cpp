#include "cliptile.hpp"

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
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
