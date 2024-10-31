#ifdef _WIN32 // Only for windows using wintoast see https://github.com/mohabouje/WinToast

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "joinrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::notify {
/**
 * @brief Accept Impl
 */
void JoinRequest::acceptImpl() const {
  QMetaObject::invokeMethod(const_cast<JoinRequest*>(this), "onAccept", Qt::QueuedConnection);
}

/**
 * @brief Reject Impl
 */
void JoinRequest::rejectImpl() const {
  QMetaObject::invokeMethod(const_cast<JoinRequest*>(this), "onReject", Qt::QueuedConnection);
}

/**
 * @brief Called when user Activated
 */
void JoinRequest::toastActivated() const {
  this->rejectImpl();
}

/**
 * @brief Called when user Failed
 */
void JoinRequest::toastFailed() const {
  this->rejectImpl();
}

/**
 * @brief Called when user Dismissed
 */
void JoinRequest::toastDismissed(WinToastDismissalReason state) const {
  if (state == WinToastDismissalReason::UserCanceled) this->rejectImpl();
}

/**
 * @brief Called when user Activated
 */
void JoinRequest::toastActivated(int actionIndex) const {
  actionIndex == 0 ? this->acceptImpl() : this->rejectImpl();
}

/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
JoinRequest::JoinRequest(QObject *parent) : QObject(parent) {
  // Do nothing
}

/**
 * @brief Show the notification
 */
void JoinRequest::show(const types::device::Device &device) {
  auto toast = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::ImageAndText01);
  auto text  = QObject::tr("%1 wants to Join to your Group").arg(device.name);
  toast.setTextField(text.toStdWString(), WinToastLib::WinToastTemplate::FirstLine);

  std::vector<std::wstring> actions{
    tr("Accept").toStdWString(),    // 0
    tr("Reject").toStdWString(),    // 1
  };

  for (auto const &action : actions) {
    toast.addAction(action);
  }

  WinToastLib::WinToast::instance()->showToast(toast, this);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::notify
#endif  // _WIN32
