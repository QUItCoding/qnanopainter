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
        border.color: "#b0b0b0"
        color: mouseArea.pressed ? "#808080" : "#606060"
        opacity:  !root.isToggle || root.toggled ? 1.0 : 0.4
    }

    Text {
        id: textItem
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: root.width - 8
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.pixelSize: 10 * dp
        color: "#ffffff"
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

