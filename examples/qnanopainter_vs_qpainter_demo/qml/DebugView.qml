import QtQuick 2.0

Item {
    id: root

    property int drawCallCount: qnItem ? qnItem.debug.drawCallCount : 0
    property int strokeTriCount: qnItem ? qnItem.debug.strokeTriCount : 0
    property int fillTriCount: qnItem ? qnItem.debug.fillTriCount : 0
    property int textTriCount: qnItem ? qnItem.debug.textTriCount : 0

    height: infoContent.height + 20 * dp
    width: infoContent.width + 40 * dp

    component InfoTextItem: Text {
        font.pixelSize: 18 * dp
        color: "#d0d0d0"
    }

    Rectangle {
        anchors.fill: parent
        color: "#404040"
    }

    Column {
        id: infoContent
        x: 20 * dp
        y: 10 * dp
        InfoTextItem {
            text: "DRAW CALLS: <b>" + root.drawCallCount + "</b>"
        }
        InfoTextItem {
            text: "FILL TRIANGLES: <b>" + root.fillTriCount + "</b>"
        }
        InfoTextItem {
            text: "STROKE TRIANGLES: <b>" + root.strokeTriCount + "</b>"
        }
        InfoTextItem {
            text: "TEXT TRIANGLES: <b>" + root.textTriCount + "</b>"
        }
    }
}
