// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "utilities.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Authenticator for server to authenticate the client
 * when client connects to server, This Authenticator just
 * shows a info of the client and asks for permission to
 * connect or not.
 *
 * @param host <QHostAddress, quint16>
 *
 * @return true if user accepts the connection
 * @return false if user rejects the connection
 */
bool authenticator(QPair<QHostAddress, quint16> host) {
  // get the message to show
  auto message = QString("Host: %1\nPort: %2\n\nAccept?").arg(
      host.first.toString(), QString::number(host.second));

  // title of the dialog
  auto title = "New Client Connection!";

  // get the user input
  auto dialog = QInputDialog();

  // Set the dialog properties
  dialog.setWindowFlags(Qt::WindowStaysOnTopHint);
  dialog.setWindowModality(Qt::ApplicationModal);
  dialog.setFixedSize(dialog.sizeHint());

  // set the dialog message
  dialog.setWindowTitle(title);
  dialog.setLabelText(message);

  // set the dialog buttons
  dialog.setOkButtonText("Accept");
  dialog.setCancelButtonText("Reject");

  // show the dialog and return the status
  return dialog.exec() == QDialog::Accepted;
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::utility
