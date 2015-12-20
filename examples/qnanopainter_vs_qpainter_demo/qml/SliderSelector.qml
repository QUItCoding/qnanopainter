import QtQuick 2.0

Item {
    id: root

    // The title text of selector
    property string title: ""
    // Array of strings for labels
    property var texts: []
    // Index (0..texts.length) of currently selected item
    property int selectedIndex: 0

    width: parent.width
    height: 100 * dp

    Text {
        id: titleItem
        y: 8
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 16 * dp
        color: "#ffffff"
        text: root.title
    }

    Row {
        anchors.top: titleItem.bottom
        anchors.topMargin: 4 * dp
        width: parent.width
        height: parent.height
        Repeater {
            model: root.texts.length
            Item {
                width: parent.width / root.texts.length
                height: width
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width * 0.9
                    height: width
                    radius: width/2
                    border.color: "#808080"
                    color: root.selectedIndex == index ? "#e0e0e0" : "#606060"
                    Text {
                        id: itemTitle
                        anchors.centerIn: parent
                        font.pixelSize: 16 * dp
                        color: "#202020"
                        text: root.texts[index]
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    anchors.topMargin: -10 * dp
                    anchors.bottomMargin: -10 * dp
                    onClicked: root.selectedIndex = index;
                }
            }
        }
    }
}


