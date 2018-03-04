import QtQuick 2.9

Item {
    id: root

    property real time: 0
    property int items: 10

    readonly property real dx: root.width / items
    readonly property real barWidth: dx * 0.8
    readonly property real margin: dx - barWidth

    Repeater {
        model: items
        Rectangle {
            anchors.bottom: parent.bottom
            x: index * dx + margin/2
            width: root.barWidth
            height: root.height * (0.5 + Math.sin(index*0.1+time)*0.5)
            color: m_color3
            border.color: m_colorBlack
            border.width: 1
        }
    }
}
