// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/**
   * @brief set the host as client
   */
  void setHostAsClient(QSslConfiguration config ) {
    // check the QSslConfiguration is set
    if (m_sslConfig.isNull()) {
      throw std::runtime_error("SSL Config is not set");
    }

    // Emplace the client into the m_host variant variable
    auto *client = &m_host.emplace<SyncingClient>(this);

    // Connect the onServerListChanged signal to the signal
    const auto signal_s = &SyncingClient::OnServerListChanged;
    const auto slot_s = &Controller::OnServerListChanged;
    connect(client, signal_s, this, slot_s);

    // Connect the onServerFound signal to the signal
    const auto signal_f = &SyncingClient::OnServerFound;
    const auto slot_f = &Controller::OnServerFound;
    connect(client, signal_f, this, slot_f);

    // Connect the onServerStateChanged signal to the signal
    const auto signal_c = &SyncingClient::OnServerStateChanged;
    const auto slot_c = &Controller::OnServerStateChanged;
    connect(client, signal_c, this, slot_c);

    // Connect the onErrorOccurred signal to the signal
    const auto signal_e = &SyncingClient::OnErrorOccurred;
    const auto slot_e = &Controller::OnErrorOccurred;
    connect(client, signal_e, this, slot_e);

    // Connect the onSyncRequest signal to the clipboard
    const auto signal_r = &SyncingClient::OnSyncRequest;
    const auto slot_r = &clipboard::Clipboard::set;
    connect(client, signal_r, &m_clipboard, slot_r);

    // Connect the onClipboardChange signal to the client
    const auto signal_b = &clipboard::Clipboard::OnClipboardChange;
    const auto slot_b = &SyncingClient::syncItems;
    connect(&m_clipboard, signal_b, client, slot_b);
  }
