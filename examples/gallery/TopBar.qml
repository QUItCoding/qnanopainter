import QtQuick 2.0

Item {
    id: root

    property int currentIndex: 0
    property int itemCount: 0
    property string title

    onCurrentIndexChanged: {
        switch(root.currentIndex) {
        case 0:
            title = "Colors and Brushes";
            break;
        case 1:
            title = "Paths, Caps & Joins";
            break;
        case 2:
            title = "States, Transitions and Clipping";
            break;
        case 3:
            title = "Antialiasing and Line Width";
            break;
        case 4:
            title = "Text: Fonts and Styles";
            break;
        case 5:
            title = "Text: Alignment and Wrapping";
            break;
        case 6:
            title = "Images";
            break;
        }
    }

    width: parent.width
    height: Math.floor(70 * dp)

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        opacity: 0.2
    }
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: titleTextItem.top
        font.pixelSize: 12 * dp
        color: "#909090"
        text: "QNanoPainter Gallery"
    }
    Text {
        id: titleTextItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 20 * dp
        color: "#e0e0e0"
        text: title
    }
    Row {
        anchors.top: titleTextItem.bottom
        anchors.topMargin: 8 * dp
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 6 * dp
        Repeater {
            model: root.itemCount
            Rectangle {
                width: 8 * dp
                height: width
                radius: width/2
                color: "#e0e0e0"
                border.width: 1
                border.color: "#000000"
                opacity: index == root.currentIndex ? 0.8 : 0.2
                Behavior on opacity {
                    NumberAnimation {
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }
}

