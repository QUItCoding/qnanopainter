import QtQuick 2.3
import PaintingItem 1.0

Item {
    id: mainView

    property int margin: Math.min(width, height)*0.02

    Item {
        id: topArea
        width: parent.width
        height: mainWindow.topbarHeight
        Text {
            anchors.centerIn: parent
            font.pixelSize: 20 * dp
            color: "#f0f0f0"
            text: "Just draw below..."
        }
    }

    PaintingItem {
        id: paintingItem
        anchors.top: topArea.bottom
        anchors.topMargin: margin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: margin
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - margin*2
        mouseEventsEnabled: true
    }

}
