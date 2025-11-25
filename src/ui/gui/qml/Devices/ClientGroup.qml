pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import Clipbird.Qml

Item {
    id: root

    property var connectedServer: ClipbirdQmlSyncingManager.connectedServer
    property var availableServers: {
        return ClipbirdQmlSyncingManager.availableServers.filter(server => server.name !== root.connectedServer?.name);
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: !connectedServer && availableServers.length === 0 ? 0 : 1

        Rectangle {
            color: "transparent"

            Text {
                anchors.centerIn: parent
                text: qsTr("No servers available.")
                font.pixelSize: 14
                color: Material.secondaryTextColor
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }

        Column {
            spacing: 0

            Loader {
                width: parent.width
                active: root.connectedServer !== null
                visible: active

                sourceComponent: ClientServerSession {
                    session: root.connectedServer

                    onDisconnectClicked: {
                        ClipbirdQmlSyncingManager.disconnectFromServer()
                    }

                    onAddToTrustClicked: {
                        if (root.connectedServer) {
                            ClipbirdQmlTrustedServers.addTrustedServer(root.connectedServer.name, root.connectedServer.certificate)
                        }
                    }
                }
            }

            ListView {
                width: parent.width
                height: root.connectedServer ? parent.height - y : parent.height
                model: root.availableServers
                spacing: 0
                clip: true

                delegate: ClientServer {
                    required property var modelData
                    width: ListView.view.width
                    server: modelData

                    onJoinClicked: {
                        ClipbirdQmlSyncingManager.connectToServer(modelData.name)
                    }
                }
            }
        }
    }
}
