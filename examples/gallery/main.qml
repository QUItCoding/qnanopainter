import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    id: mainWindow

    property real dp: Math.min(width, height) / 400
    property string title: ""
    property real topbarHeight: Math.floor(50 * dp)

    width: 375
    height: 667
    visible: true
    color: "#404040"

    ListModel {
        id: galleryModel
        ListElement {
            name: "QNanoPainter Features"
            path: "qnanopainter_features"
        }
        ListElement {
            name: "Mouse Event Boxes"
            path: "mouse_events"
        }
        ListElement {
            name: "Freehand Painting"
            path: "painting"
        }
    }

    ListView {
        id: listView
        anchors.top: topSecondArea.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        orientation: ListView.Vertical
        maximumFlickVelocity: 10000
        highlightMoveDuration: 2500
        model: galleryModel
        visible: opacity
        opacity: galleryLoader.source == ""
        Behavior on opacity {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
        delegate: Item {
            width: listView.width
            height: 75 * dp
            Text {
                id: delegateTextItem
                anchors.centerIn: parent
                font.pixelSize: 20 * dp
                color: "#b0b0b0"
                text: model.name
                opacity: delegateMouseArea.pressed ? 0.4 : 1.0
            }
            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent
                onClicked: {
                    mainWindow.title = model.name;
                    galleryLoader.loadDemo(model.path);
                }
            }
            Image {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 25 * dp
                width: 20 * dp
                height: 20 * dp
                rotation: 180
                source: "back.png"
                opacity: delegateMouseArea.pressed ? 0.4 : 1.0
            }
            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                width: parent.width * 0.9
                height: Math.ceil(1 * dp)
                color: "#606060"
            }
        }
    }

    Rectangle {
        id: topSecondArea
        anchors.top: topArea.bottom
        width: parent.width
        height: mainWindow.topbarHeight
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#303030" }
            GradientStop { position: 0.4; color: "#101010" }
            GradientStop { position: 1.0; color: "#292929" }
        }
        Text {
            id: titleTextItem
            anchors.centerIn: parent
            font.pixelSize: 14 * dp
            font.bold: true
            color: "#f0f0f0"
            text: "QNanoPainter Examples Gallery"
            visible: galleryLoader.source == ""
        }
    }

    Loader {
        id: galleryLoader

        function loadDemo(path) {
            galleryLoader.source = path + "/main.qml";
        }

        function closeDemo() {
            mainWindow.title = "";
            galleryLoader.source = "";
        }

        anchors.top: topArea.bottom
        anchors.bottom: parent.bottom
        width: parent.width

        Behavior on source {
            SequentialAnimation {
                NumberAnimation {
                    target: galleryLoader
                    property: "opacity"
                    to: 0
                    duration: galleryLoader.source == "" ? 0 : 500
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    target: galleryLoader
                    property: "source"
                }
                NumberAnimation {
                    target: galleryLoader
                    property: "opacity"
                    to: 1
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Rectangle {
        id: topArea
        width: parent.width
        height: Math.floor(25 * dp)
        color: "#404040"
        Text {
            id: textItem
            anchors.centerIn: parent
            font.pixelSize: 12 * dp
            color: "#C0C0C0"
            text: mainWindow.title
            Behavior on text {
                SequentialAnimation {
                    NumberAnimation {
                        target: textItem
                        property: "anchors.verticalCenterOffset"
                        to: -20 * dp
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                    PropertyAction {
                        target: textItem
                        property: "text"
                    }
                    NumberAnimation {
                        target: textItem
                        property: "anchors.verticalCenterOffset"
                        to: 0
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }

    Image {
        property real animationState: galleryLoader.source != ""
        anchors.verticalCenter: topSecondArea.verticalCenter
        width: 20 * dp
        height: 20 * dp
        x: animationState * 5 * dp
        source: "back.png"
        enabled: opacity
        opacity: backMouseArea.pressed ? 0.4 : 1.0 * animationState
        Behavior on animationState {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
        MouseArea {
            id: backMouseArea
            anchors.fill: parent
            anchors.margins: -15 * dp
            onClicked: {
                galleryLoader.closeDemo();
            }
        }
    }
}
