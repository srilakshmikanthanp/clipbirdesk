#pragma once

#include <QString>

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct DeviceType {
  static constexpr const char* ANDROID = "ANDROID";
  static constexpr const char* WINDOWS = "WINDOWS";
  static constexpr const char* LINUX   = "LINUX";

  static QString getCurrentDeviceType() {
#ifdef Q_OS_ANDROID
    return QString(ANDROID);
#elif defined(Q_OS_WIN)
    return QString(WINDOWS);
#elif defined(Q_OS_LINUX)
    return QString(LINUX);
#else
    return throw std::runtime_error("Unsupported OS");
#endif
  }
};
};
