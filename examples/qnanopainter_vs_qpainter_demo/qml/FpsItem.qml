import QtQuick 2.0

Item {
    id: root
    property int frameCounter: 0
    property int frameCounterAvg: 0
    property int counter: 0
    property int fps: 0
    property int fpsAvg: 0

    width: 140 * dp
    height: Math.floor(48 * dp)

    Image {
        id: spinnerImage
        anchors.verticalCenter: parent.verticalCenter
        x: 4 * dp
        width: 36 * dp
        height: width
        source: "images/spinner.png"
        NumberAnimation on rotation {
            from:0
            to: 360
            duration: 800
            loops: Animation.Infinite
        }
        onRotationChanged: frameCounter++;
    }
    Image {
        anchors.centerIn: spinnerImage
        width: 20 * dp
        height: width
        source: animationTimeAnimation.paused ? "images/ic_pause_white_48dp.png" : "images/ic_play_arrow_white_48dp.png"
        MouseArea {
            anchors.fill: parent
            anchors.margins: -16 * dp
            onClicked: {
                animationTimeAnimation.paused = !animationTimeAnimation.paused
            }
        }
    }

    Text {
        anchors.left: spinnerImage.right
        anchors.leftMargin: 8 * dp
        anchors.verticalCenter: spinnerImage.verticalCenter
        color: "#c0c0c0"
        //style: Text.Outline
        //styleColor: "#606060"
        font.pixelSize: 18 * dp
        text: "Ø " + root.fpsAvg + " | " + root.fps + " fps"
    }

    Timer {
        interval: 2000
        repeat: true
        running: true
        onTriggered: {
            frameCounterAvg += frameCounter;
            root.fps = frameCounter/2;
            counter++;
            frameCounter = 0;
            if (counter >= 3) {
                root.fpsAvg = frameCounterAvg/(2*counter)
                frameCounterAvg = 0;
                counter = 0;
            }
        }
    }
}
