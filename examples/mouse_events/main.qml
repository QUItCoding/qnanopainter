import QtQuick 2.3
import QtQuick.Window 2.2
import EventItem 1.0

Window {
    id: mainWindow

    property int margin: Math.min(width, height)*0.02

    visible: true
    width: 480
    height: 800

    Rectangle {
        anchors.fill: parent
        color: "#202020"
        MouseArea {
            id: mouseArea
            anchors.fill: parent
        }
        Text {
            id: textItem
            y: margin
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Press going through"
            font.pointSize: 24
            color: "#ffffff"
            opacity: mouseArea.pressed ? 1.0 : 0.2
        }
        Row {
            id: buttonArea
            anchors.top: textItem.bottom
            anchors.topMargin: margin
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - margin*2
            height: parent.height * 0.06
            spacing: margin * 3/2
            ToggleButton {
                id: enableMouseToggle
                width: parent.width / 3 - margin
                text: "Enable mouse"
                toggled: true
            }
            ToggleButton {
                id: enableHoverToggle
                width: parent.width / 3 - margin
                text: "Enable hover"
                toggled: true
                onToggledChanged: {
                    eventItem.update();
                }
            }
            ToggleButton {
                width: parent.width / 3 - margin
                text: "Random"
                isToggle: false
                onClicked: {
                    eventItem.generateRandomItems();
                }
            }
        }

        EventItem {
            id: eventItem
            anchors.top: buttonArea.bottom
            anchors.topMargin: margin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: margin
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - margin*2
            mouseEventsEnabled: enableMouseToggle.toggled
            mouseHoverEventsEnabled: enableHoverToggle.toggled
            //acceptedButtons: Qt.AllButtons
        }
    }
}
