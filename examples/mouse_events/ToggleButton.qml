import QtQuick 2.0

Item {
    id: root

    property bool isToggle: true
    property bool toggled: false
    property alias text: textItem.text
    signal clicked()

    width: 100
    height: parent.height

    Rectangle {
        anchors.fill: parent
        border.width: 1
        border.color: "#f0f0f0"
        color: !root.isToggle || root.toggled ? "#e0e0e0" : "#808080"
        opacity: mouseArea.pressed ? 0.8 : 1.0
    }

    Text {
        id: textItem
        anchors.verticalCenter: parent.verticalCenter
        width: root.width - 8
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.pointSize: 14
        color: "#202020"
        opacity: root.toggled || !root.isToggle ? 1.0 : 0.5
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if (root.isToggle) {
                root.toggled = !root.toggled;
            } else {
                root.clicked();
            }
        }
    }
}

