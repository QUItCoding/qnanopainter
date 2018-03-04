import QtQuick 2.0

Item {
    id: root

    property alias text: textItem.text
    property bool checked: false
    property string textOn: "ON"
    property string textOff: "OFF"

    signal userChecked()

    QtObject {
        id: priv
        property real switchWidth: Math.max(66*dp, textOnItem.paintedWidth + 50*dp)
        property real barHeight: 19 * dp
        property real knobMovement: switchWidth - knobSize + 2
        property real knobSize: 32 * dp
        property real knobState: knob.x / knobMovement

        function doSwitch() {
            root.checked = !root.checked;
            root.userChecked();
        }

        function releaseSwitch() {
            // Don't switch if we are in correct side
            if ((knob.x == -2 && !checked) || (knob.x == priv.knobMovement && checked)) {
                return;
            }
            priv.doSwitch();
        }
    }

    width: parent ? parent.width : 200 * dp
    height: 60 * dp

    MouseArea {
        width: parent.width
        height: parent.height
        onClicked: priv.doSwitch();
    }

    Text {
        id: textItem
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20 * dp
        anchors.right: switchBackgroundImage.left
        anchors.rightMargin: 20 * dp
        //horizontalAlignment: Text.AlignHCenter
        horizontalAlignment: Text.AlignRight
        elide: Text.ElideRight
        font.pixelSize: 16 * dp
        color: "#ffffff"
        style: Text.Outline
        styleColor: "#000000"
    }

    Rectangle {
        id: switchBackgroundImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 32 * dp
        height: priv.barHeight
        width: priv.switchWidth
        radius: height/8
        color: "#404040"
    }
    Rectangle {
        id: switchFrameImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 32 * dp
        height: priv.barHeight
        width: priv.switchWidth
        radius: height/8
        color: "transparent"
        border.width: 1 * dp
        border.color: "#808080"
        z: 2
    }

    Item {
        id: switchItem
        anchors.fill: switchBackgroundImage
        clip: true

        Text {
            id: textOnItem
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: knob.left
            anchors.rightMargin: 6
            color: "#ffffff"
            font.pixelSize: 12 * dp
            text: textOn
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: knob.right
            anchors.leftMargin: 4
            color: "#808080"
            font.pixelSize: 12 * dp
            text: textOff
        }

        Item {
            id: knob
            anchors.verticalCenter: parent.verticalCenter
            height: priv.knobSize
            width: height
            x: checked ? priv.knobMovement : -2
            MouseArea {
                anchors.fill: parent
                drag.target: knob; drag.axis: Drag.XAxis; drag.minimumX: -2; drag.maximumX: priv.knobMovement
                onClicked: priv.doSwitch();
                onReleased: priv.releaseSwitch();
            }
            Behavior on x {
                NumberAnimation { duration: 250; easing.type: Easing.InOutQuad }
            }
        }
    }

    Rectangle {
        id: knobVisual
        property real colorValue: 0.6 + priv.knobState*0.4
        anchors.verticalCenter: parent.verticalCenter
        height: priv.knobSize
        width: height
        x: switchBackgroundImage.x + knob.x
        z: 10
        radius: height/2
        color: Qt.rgba(colorValue, colorValue, colorValue, 1.0)
        border.width: 1 * dp
        border.color: "#404040"
    }
}
