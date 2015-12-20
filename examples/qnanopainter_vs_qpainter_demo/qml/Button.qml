import QtQuick 2.0

Item {
    id: root

    property alias text: textItem.text
    signal clicked

    height: 42 * dp
    width: 136 * dp

    Rectangle {
        anchors.fill: parent
        color: "#808080"
        border.color: "#d0d0d0"
        border.width: 1
        opacity: 0.5
    }

    Text {
        id: textItem
        anchors.centerIn: parent
        color: "#f0f0f0"
        font.pixelSize: 18 * dp
    }
    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked();
    }
}

