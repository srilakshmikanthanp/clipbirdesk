pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: menuContainer

    required property var model
    required property int selectedIndex
    signal menuClicked(int index)

    color: Material.backgroundColor
    Layout.minimumWidth: parent.width / 4

    Rectangle {
        anchors.right: parent.right
        width: 1
        height: parent.height
        color: Material.dividerColor
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: menuContainer.model
        clip: true

        delegate: Column {
            id: delegateColumn

            required property var modelData
            required property int index

            width: listView.width
            spacing: 0

            ItemDelegate {
                id: delegateItem
                width: listView.width
                text: delegateColumn.modelData.name
                highlighted: delegateColumn.index === menuContainer.selectedIndex
                Layout.fillWidth: true
                padding: 12
                onClicked: menuContainer.menuClicked(delegateColumn.index)
                background: Rectangle {
                    color: delegateColumn.index === menuContainer.selectedIndex ? palette.highlight : "transparent"
                }
            }
        }

        footer: Item {
            Layout.fillHeight: true
        }
    }
}
