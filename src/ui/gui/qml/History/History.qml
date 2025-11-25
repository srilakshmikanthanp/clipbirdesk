import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Clipbird.Qml

Page {
    id: root
    title: qsTr("History")

    property var history: ClipbirdQmlHistory.history

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        SendClipboardCard {
            Layout.fillWidth: true
            onSendClicked: {
                ClipbirdQmlSyncingManager.synchronize(ClipbirdQmlApplicationClipboard.getClipboard());
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: root.history.length === 0 ? 0 : 1

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Column {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        text: qsTr("No clipboard history yet")
                        font.pixelSize: 16
                        color: Material.primaryTextColor
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        text: qsTr("Your clipboard history will appear here")
                        font.pixelSize: 14
                        color: Material.secondaryTextColor
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ListView {
                    anchors.fill: parent
                    model: root.history
                    spacing: 10

                    delegate: HistoryItem {
                        required property int index
                        required property var modelData
                        width: ListView.view.width
                        historyData: modelData

                        onCopyClicked: {
                            ClipbirdQmlApplicationClipboard.setClipboard(modelData)
                        }

                        onDeleteClicked: {
                            ClipbirdQmlHistory.deleteHistoryAt(index)
                        }
                    }
                }
            }
        }
    }
}
