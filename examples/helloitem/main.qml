import QtQuick 2.3
import QtQuick.Window 2.2
import HelloItem 1.0

Window {
    visible: true
    width: 480
    height: 800
    Text {
        id: textItem
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height * 0.02
        font.pixelSize: parent.width * 0.04
        color: "#404040"
        text: "Here is our custom QNanoPainter item"
    }

    HelloItem {
        anchors.top: textItem.bottom
        anchors.bottom: parent.bottom
        width: parent.width
    }
}
