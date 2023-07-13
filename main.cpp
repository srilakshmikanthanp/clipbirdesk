#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>


class UndecoratedWindow : public QWidget {
public:
  UndecoratedWindow(): QWidget(nullptr, Qt::Tool | Qt::FramelessWindowHint) {
    // add 3 buttons vertically
    QPushButton* button1 = new QPushButton("Button 1");
    QPushButton* button2 = new QPushButton("Button 2");
    QPushButton* button3 = new QPushButton("Button 3");

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);

    // hide on taskbar


    this->setLayout(layout);
  }
};

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  UndecoratedWindow window;
  window.show();

  return app.exec();
}
