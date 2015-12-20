import QtQuick 2.4
import QtQuick.Window 2.2
import NanoVGQuick 1.0

Window {
    id: mainWindow

    property real animationTime: 0
    property real animationSine: 0
    property real dp: Math.min(width, height) / 400

    width: 480
    height: 800
    visible: true
    color: "#404040"

    NumberAnimation on animationTime {
        id: animationTimeAnimation
        from: 0
        to: 360
        duration: 1000*360
        loops: Animation.Infinite
    }
    SequentialAnimation on animationSine {
        loops: Animation.Infinite
        NumberAnimation {
            from: 0
            to: 1
            duration: 1000
            easing.type: Easing.InOutSine
        }
        NumberAnimation {
            from: 1
            to: 0
            duration: 1000
            easing.type: Easing.InOutSine
        }
    }

    TopBar {
        id: topBar
        currentIndex: listView.currentIndex
        itemCount: listView.count
    }

/*
    // This can be used for autoscrolling gallery to see memory usage
    // while (un)loading items
    SequentialAnimation {
        running: true
        loops: Animation.Infinite
        ScriptAction {
            script: listView.currentIndex = listView.count-1;
        }
        PauseAnimation {
            duration: 5000
        }
        ScriptAction {
            script: listView.currentIndex = 0;
        }
        PauseAnimation {
            duration: 5000
        }
    }
*/

    ListView {
        id: listView
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        orientation: ListView.Horizontal
        snapMode: ListView.SnapToItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        maximumFlickVelocity: 10000
        highlightMoveDuration: 2500
        model: 7
        delegate: GalleryItem {
            width: listView.width
            height: listView.height
            animationTime: mainWindow.animationTime
            animationSine: mainWindow.animationSine
            galleryView: index
        }
    }
}
