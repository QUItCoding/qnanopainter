import QtQuick 2.9

Item {
    id: root

    property real time: 0
    property int items: 0

    readonly property real size: width/12
    readonly property real fontSize: width/22

    function posX(i) {
        return (root.width - size) / items*i;
    }
    function posY(i) {
        return height * 0.5 + height * Math.sin((i+1) * time * 0.1) * 0.5;
    }

    Repeater {
        model: items
        Image {
            source: "../images/circle.png"
            width: size
            height: size
            y: posY(index)
            x: posX(index)
            Text {
                anchors.centerIn: parent
                font.pixelSize: fontSize
                font.family: m_testFont.name
                color: "#FFFFFF"
                text: (index+1)
            }
        }
    }
}
