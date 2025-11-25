pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import Clipbird.Qml

Item {
    id: root
    width: parent.width
    height: 150

    required property var historyData
    signal copyClicked
    signal deleteClicked

    Card {
        anchors.fill: parent
        anchors.margins: 4

        Column {
            anchors.fill: parent
            spacing: 5

            HistoryItemContent {
                width: parent.width
                height: parent.height * 0.70
                historyData: root.historyData
            }

            HistoryItemAction {
                width: parent.width
                height: parent.height * 0.30
                onCopyClicked: root.copyClicked()
                onDeleteClicked: root.deleteClicked()
            }
        }
    }
}
