#include "platformclipboard.hpp"

#include <QGuiApplication>

#ifdef __linux__
#include "clipboard/waylandclipboard.hpp"
#endif

#include "clipboard/qtclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::clipboard {
PlatformClipboard::PlatformClipboard(QObject *parent) : QObject(parent) {}

PlatformClipboard *PlatformClipboard::instance() {
  static PlatformClipboard *instance = nullptr;

#ifdef __linux__
  if (instance == nullptr && qGuiApp->platformName() == QLatin1String("wayland")) {
    instance = new WaylandClipboard(qGuiApp);
  }
#endif

  if (instance == nullptr) {
    instance = new QtClipboard(qGuiApp);
  }

  return instance;
}

QString PlatformClipboard::text(QClipboard::Mode mode) {
  return instance()->mimeData(mode)->text();
}
}  // namespace srilakshmikanthanp::clipbirdesk::clipboard