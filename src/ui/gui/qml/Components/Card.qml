import QtQuick
import QtQuick.Effects
import QtQuick.Controls.Material

Rectangle {
    id: root

    default property alias content: contentItem.data

    color: Material.dialogColor
    radius: 8
    border.color: Material.frameColor
    border.width: 1

    Item {
        id: contentItem
        anchors.fill: parent
        anchors.margins: 10
    }

    layer.enabled: true
    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowHorizontalOffset: 0
        shadowVerticalOffset: 2
        shadowBlur: 0.4
        shadowOpacity: 0.3
    }
}
