#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <iostream>
#include <QMimeDatabase>

#include "clipboard/clipboard.hpp"

int main(int argc, char** argv) {
  auto application = QApplication(argc, argv);
  auto clip = QApplication::clipboard();

  const auto onChange = [&]() {
    const auto mimeData = clip->mimeData();
    // if (mimeData->hasText()) {
    //   const auto data = mimeData->data("text/plain");
    //   const auto text = QString::fromUtf8(data);
    //   std::cout << text.toStdString() << std::endl;
    // }

    if (mimeData->hasImage()) {
      const auto image = mimeData->formats();
      for (const auto& format : image) {
        std::cout << format.toStdString() << std::endl;
      }
    }
  };

  auto signal = &QClipboard::changed;
  QObject::connect(clip, signal, onChange);
  application.exec();
}
