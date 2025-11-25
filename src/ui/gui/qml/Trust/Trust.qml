import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Clipbird.Qml

Page {
    id: root
    title: qsTr("Trusted Devices")
    property int tabIndex: 0

    header: Column {
        width: parent.width

        TabBar {
            id: tabBar
            currentIndex: root.tabIndex
            focusPolicy: Qt.NoFocus

            TabButton {
                text: qsTr("Trusted Groups")
                width: implicitWidth
                background: Rectangle {
                    color: "transparent"
                }
                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
            }

            TabButton {
                text: qsTr("Trusted Members")
                width: implicitWidth
                background: Rectangle {
                    color: "transparent"
                }
                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
            }

            onCurrentIndexChanged: root.tabIndex = currentIndex
        }
    }

    ScrollView {
        anchors.fill: parent

        StackLayout {
            anchors.fill: parent
            currentIndex: root.tabIndex

            TrustedDevices {
                id: serversView
                devices: ClipbirdQmlTrustedServers.getTrustedServers()
                emptyMessage: qsTr("No trusted Group found.")
                onRemoveClicked: name => ClipbirdQmlTrustedServers.removeTrustedServer(name)

                Connections {
                    target: ClipbirdQmlTrustedServers
                    function onTrustedServersChanged(servers) {
                        serversView.devices = servers;
                    }
                }
            }

            TrustedDevices {
                id: clientsView
                devices: ClipbirdQmlTrustedClients.getTrustedClients()
                emptyMessage: qsTr("No trusted Members found.")
                onRemoveClicked: name => ClipbirdQmlTrustedClients.removeTrustedClient(name)

                Connections {
                    target: ClipbirdQmlTrustedClients
                    function onTrustedClientsChanged(clients) {
                        clientsView.devices = clients;
                    }
                }
            }
        }
    }
}
