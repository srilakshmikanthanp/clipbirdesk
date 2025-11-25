pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import Clipbird.Qml

Item {
    id: root
    width: parent.width

    required property var historyData

    property var contentComponent: null
    property var contentItem: null

    Component.onCompleted: {
        for (let i = 0; i < root.historyData.length; i++) {
            const item = root.historyData[i];
            if (item.mimeType === "image/png") {
                contentItem = item;
                contentComponent = imageComponent;
                return;
            }
        }

        for (let i = 0; i < root.historyData.length; i++) {
            const item = root.historyData[i];
            if (item.mimeType === "text/plain") {
                contentItem = item;
                contentComponent = textComponent;
                return;
            }
        }
    }

    Loader {
        anchors.fill: parent
        sourceComponent: root.contentComponent
    }

    Component {
        id: imageComponent
        Image {
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: ClipbirdQmlImageHelper.imageUrl(root.contentItem.data)
        }
    }

    Component {
        id: textComponent
        Text {
            text: root.contentItem.data.toString(Qt.CompatUtf8)
            color: Material.primaryTextColor
            wrapMode: Text.WordWrap
            elide: Text.ElideRight
            maximumLineCount: 4
            width: parent.width
        }
    }
}
