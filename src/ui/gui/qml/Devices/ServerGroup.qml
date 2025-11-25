pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import Clipbird.Qml

Item {
    id: root

    StackLayout {
        anchors.fill: parent
        currentIndex: ClipbirdQmlSyncingManager.connectedClients.length === 0 ? 0 : 1

        Rectangle {
            color: "transparent"

            Text {
                anchors.centerIn: parent
                text: qsTr("No clients connected.")
                font.pixelSize: 14
                color: Material.secondaryTextColor
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }

        ListView {
            model: ClipbirdQmlSyncingManager.connectedClients
            spacing: 0
            clip: true

            delegate: ServerClientSession {
                required property var modelData
                width: ListView.view.width
                session: modelData

                onDisconnectClicked: {
                    ClipbirdQmlSyncingManager.disconnectClient(modelData.name)
                }

                onAddToTrustClicked: {
                    ClipbirdQmlTrustedClients.addTrustedClient(modelData.name, modelData.certificate)
                }
            }
        }
    }
}
