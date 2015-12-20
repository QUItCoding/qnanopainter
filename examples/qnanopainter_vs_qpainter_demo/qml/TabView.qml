import QtQuick 2.0

Item {
    id: root
    property int currentIndex: 0
    property var texts: []
    property int itemCount: texts.length

    width: parent.width
    height: 60 * dp

    Row {
        Repeater {
            model: itemCount
            Item {
                height: root.height
                width: root.width / itemCount
                Rectangle {
                    anchors.fill: parent
                    color: currentIndex == index ? "#000000" : "#202020"
                    opacity: 0.75
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.9
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20 * dp
                    color: "#ffffff"
                    text: root.texts[index]
                    opacity: currentIndex == index ? 1.0 : 0.5
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked:  {
                        root.currentIndex = index;
                    }
                }
            }
        }
    }
}

