import QtQuick 2.0

Item {
    id: root

    property var model: []
    property int currentIndex: 0
    property bool isOpen: false
    property real isOpenAnimated: isOpen

    height: 42 * dp
    width: 136 * dp

    Behavior on isOpenAnimated {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
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
        text: model.length > 0 ? model[currentIndex] : ""
    }
    MouseArea {
        anchors.fill: parent
        onClicked: root.isOpen = !root.isOpen;
    }
    Rectangle {
        id: comboBoxPopup
        anchors.top: parent.bottom
        anchors.topMargin: 2 * dp
        width: parent.width
        height: childrenRect.height
        opacity: isOpenAnimated
        visible: opacity
        color: "#404040"
        border.color: "#d0d0d0"
        border.width: 1
        Column {
            width: parent.width
            Repeater {
                model: root.model.length
                Item {
                    width: comboBoxPopup.width
                    height: root.height
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 1
                        color: "#20ffffff"
                        visible: root.currentIndex === index
                    }
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        x: 10 * dp
                        width: parent.width
                        text: root.model[index]
                        elide: Text.ElideRight
                        color: "#ffffff"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.currentIndex = index;
                            root.isOpen = false;
                        }
                    }
                }
            }
        }
    }
}
