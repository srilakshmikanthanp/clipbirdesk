// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "joinrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification {
/**
 * @brief Called when user Dismissed
 */
void JoinRequest::toastDismissed(WinToastDismissalReason state) const {
  emit onReject();
}

/**
 * @brief Called when user Activated
 */
void JoinRequest::toastActivated() const {
  emit onReject();
}

/**
 * @brief Called when user Failed
 */
void JoinRequest::toastFailed() const {
  emit onReject();
}

/**
 * @brief Called when user Activated
 */
void JoinRequest::toastActivated(int actionIndex) const {
  switch (actionIndex) {
  case 0:
    emit onAccept();
    break;
  case 1:
    emit onReject();
    break;
  }
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
  auto text  = QObject::tr("Device %1 wants to Join to your Group").arg(device.name);

  toast.setTextField(text.toStdWString(), WinToastLib::WinToastTemplate::FirstLine);

  std::vector<std::wstring> actions;

  actions.push_back(tr("Accept").toStdWString());
  actions.push_back(tr("Reject").toStdWString());

  for (auto const &action : actions) {
    toast.addAction(action);
  }

  WinToastLib::WinToast::instance()->showToast(toast, this);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification
