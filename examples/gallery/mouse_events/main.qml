import QtQuick 2.3
import EventItem 1.0

Item {
    id: mainView

    property int margin: Math.min(width, height)*0.02

    MouseArea {
        id: mouseArea
        anchors.fill: eventItem
    }

    Item {
        id: topArea
        width: parent.width
        height: mainWindow.topbarHeight
        Row {
            id: buttonArea
            anchors.centerIn: parent
            width: parent.width * 0.75
            height: parent.height * 0.6
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
    }

    EventItem {
        id: eventItem
        anchors.top: topArea.bottom
        anchors.topMargin: margin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: margin
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - margin*2
        mouseEventsEnabled: enableMouseToggle.toggled
        mouseHoverEventsEnabled: enableHoverToggle.toggled
        //acceptedButtons: Qt.AllButtons
    }

    Text {
        id: textItem
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15 * dp
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Press going through"
        font.pixelSize: 16 * dp
        color: "#ffffff"
        opacity: mouseArea.pressed ? 1.0 : 0.2
    }
}
