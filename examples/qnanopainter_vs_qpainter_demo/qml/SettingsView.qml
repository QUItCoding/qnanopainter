import QtQuick 2.0
import DemoQNanoItem 1.0

Item {
    id: root

    property bool isOpen: false

    width: parent.width
    height: topBar.height + contentArea.height

    Rectangle {
        id: topBar
        width: parent.width
        height: fpsItem.height
        color: "#000000"
    }

    Item {
        id: contentArea
        anchors.right: parent.right
        anchors.rightMargin: 8 * dp
        anchors.top: topBar.bottom
        anchors.topMargin: 8 * dp
        width: Math.min(parent.width - 16*dp, 450*dp)
        height: tabView.height + backgroundItem.height
        visible: root.isOpen
        Rectangle {
            id: backgroundItem
            anchors.fill: tabView.currentIndex == 0 ? contentItemTab1 : contentItemTab2
            color: "#000000"
            opacity: 0.75
        }
        TabView {
            id: tabView
            anchors.top: parent.top
            texts: ["TESTS", "ADVANCED"]
        }
        Column {
            id: contentItemTab1
            anchors.top: tabView.bottom
            width: parent.width
            visible: tabView.currentIndex == 0
            Switch {
                text: "Render ruler"
                checked: true
                onCheckedChanged: {
                    enabledTests ^= 1;
                }
            }
            Switch {
                text: "Render circles"
                checked: true
                onCheckedChanged: {
                    enabledTests ^= 2;
                }
            }
            Switch {
                text: "Render bezier lines"
                checked: true
                onCheckedChanged: {
                    enabledTests ^= 4;
                }
            }
            Switch {
                text: "Render bars"
                checked: true
                onCheckedChanged: {
                    enabledTests ^= 8;
                }
            }
            Switch {
                text: "Render icons&text"
                checked: true
                onCheckedChanged: {
                    enabledTests ^= 16;
                }
            }
        }
        Column {
            id: contentItemTab2
            anchors.top: tabView.bottom
            width: parent.width
            visible: tabView.currentIndex == 1
            Switch {
                text: "Antialiasing"
                checked: qnItem.antialiasing
                onCheckedChanged: {
                    qnItem.antialiasing = checked;
                    // Note: QQuickPaintedItem supports setting
                    // antialiasing only through C++ API, QTBUG-46315
                    //qpItem.antialiasing = checked;
                    qpItem.qpAntialiasing = checked;
                    // Adjust pixelAlign accordingly
                    // Often it's preferred enabled when antialiasing is disabled
                    pixelAlignSwitch.checked = !checked;
                    // Also texts
                    pixelAlignTextSwitch.checked = !checked;
                }
            }
            Switch {
                id: pixelAlignSwitch
                text: "Pixel align paint (QNanoPainter)"
                checked: qnItem.pixelAlign !== DemoQNanoItem.PixelAlignNone
                onCheckedChanged: {
                    qnItem.pixelAlign = checked ? DemoQNanoItem.PixelAlignHalf : DemoQNanoItem.PixelAlignNone;
                }
            }
            Switch {
                id: pixelAlignTextSwitch
                text: "Pixel align text (QNanoPainter)"
                checked: qnItem.pixelAlignText !== DemoQNanoItem.PixelAlignNone
                onCheckedChanged: {
                    qnItem.pixelAlignText = checked ? DemoQNanoItem.PixelAlignFull : DemoQNanoItem.PixelAlignNone;
                }
            }
            Switch {
                text: "High quality rendering (QNanoPainter)"
                checked: qnItem.highQualityRendering
                onCheckedChanged: {
                    qnItem.highQualityRendering = checked;
                }
            }
            Switch {
                text: "FBO renderTarget (QPainter)"
                checked: qpItem.qpRenderTargetFBO
                onCheckedChanged: {
                    qpItem.qpRenderTargetFBO = checked;
                }
            }
            Switch {
                text: "Use 256x256px item"
                checked: false
                onCheckedChanged: {
                    mainWindow.fullScreen = !checked;
                    if (sizeAnimation.running) {
                        sizeAnimation.restart();
                    }
                }
            }
            Switch {
                text: "Animate size"
                checked: sizeAnimation.running
                onCheckedChanged: {
                    sizeAnimation.running = checked;
                }
            }
            SliderSelector {
                id: testCountSelector
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 40 * dp
                title: "Test rendering count"
                texts: ["1", "2", "4", "8", "16", "32", "64", "128"]
                selectedIndex: 0
                onSelectedIndexChanged: {
                    mainWindow.testCount = testCountSelector.texts[selectedIndex];
                }
            }
        }
    }

    Image {
        anchors.horizontalCenter: parent.right
        anchors.horizontalCenterOffset: -30 * dp
        anchors.top: parent.top
        anchors.topMargin: 8 * dp
        width: 32 * dp
        height: 32 * dp
        source: root.isOpen ? "images/ic_cancel_white_48dp.png" : "images/ic_settings_white_48dp.png"
        smooth: true
        MouseArea {
            anchors.fill: parent
            anchors.margins: -16 * dp
            onClicked: {
                root.isOpen = !root.isOpen;
            }
        }
    }

}

