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
            anchors.fill: tabView.currentIndex == 0 ? contentItemTab1 : tabView.currentIndex == 1 ? contentItemTab2 : contentItemTab3
            color: "#000000"
            opacity: 0.75
        }
        TabView {
            id: tabView
            anchors.top: parent.top
            texts: ["TESTS", "VIEW", "ADVANCED"]
        }
        Column {
            id: contentItemTab1
            anchors.top: tabView.bottom
            width: parent.width
            visible: tabView.currentIndex == 0
            Switch {
                text: "Render ruler"
                checked: (enabledTests & 1)
                onUserChecked: {
                    enabledTests ^= 1;
                }
            }
            Switch {
                text: "Render circles"
                checked: (enabledTests & 2)
                onUserChecked: {
                    enabledTests ^= 2;
                }
            }
            Switch {
                text: "Render bezier lines"
                checked: (enabledTests & 4)
                onUserChecked: {
                    enabledTests ^= 4;
                }
            }
            Switch {
                text: "Render bars"
                checked: (enabledTests & 8)
                onUserChecked: {
                    enabledTests ^= 8;
                }
            }
            Switch {
                text: "Render icons&text"
                checked: (enabledTests & 16)
                onUserChecked: {
                    enabledTests ^= 16;
                }
            }
            Switch {
                text: "Render flower"
                checked: (enabledTests & 32)
                onUserChecked: {
                    enabledTests ^= 32;
                }
            }
        }
        Column {
            id: contentItemTab2
            anchors.top: tabView.bottom
            width: parent.width
            visible: tabView.currentIndex == 1
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
                text: "Animate item size"
                checked: mainWindow.settingAnimateSize
                onCheckedChanged: {
                    mainWindow.settingAnimateSize = checked;
                }
            }
            SliderSelector {
                id: itemCountSelector
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 40 * dp
                title: "Item count"
                texts: ["1", "2", "4", "8", "16", "32", "64", "128"]
                selectedIndex: 0
                onSelectedIndexChanged: {
                    mainWindow.itemCount = itemCountSelector.texts[selectedIndex];
                }
            }
            SliderSelector {
                id: testCountSelector
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 40 * dp
                title: "Rendering count (per item)"
                texts: ["1", "2", "4", "8", "16", "32", "64", "128"]
                selectedIndex: 0
                onSelectedIndexChanged: {
                    mainWindow.testCount = testCountSelector.texts[selectedIndex];
                }
            }
        }
        Column {
            id: contentItemTab3
            anchors.top: tabView.bottom
            width: parent.width
            visible: tabView.currentIndex == 2
            Switch {
                text: "Antialiasing"
                checked: mainWindow.settingAntialiasing
                onCheckedChanged: {
                    mainWindow.settingAntialiasing = checked;
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
                checked: mainWindow.settingPixelAlign
                onCheckedChanged: {
                    mainWindow.settingPixelAlign = checked;
                }
            }
            Switch {
                id: pixelAlignTextSwitch
                text: "Pixel align text (QNanoPainter)"
                checked: mainWindow.settingPixelAlignText
                onCheckedChanged: {
                    mainWindow.settingPixelAlignText = checked;
                }
            }
            Switch {
                text: "High quality rendering (QNanoPainter)"
                checked: mainWindow.settingHighQualityRendering
                onCheckedChanged: {
                    mainWindow.settingHighQualityRendering = checked;
                }
            }
            Switch {
                text: "FBO renderTarget (QPainter)"
                checked: mainWindow.settingFBORendering
                onCheckedChanged: {
                    mainWindow.settingFBORendering = checked;
                }
            }
            Switch {
                text: "VendorExtensionsEnabled (QML Shape)"
                checked: mainWindow.settingVendorExtensionsEnabled
                onCheckedChanged: {
                    mainWindow.settingVendorExtensionsEnabled = checked;
                    // Force reloading Shape elements as currently vendorExtensionsEnabled
                    // can't be dynamically changed, see QTBUG-65515.
                    if (mainWindow.renderType == 2) {
                        mainWindow.renderType = 0;
                        mainWindow.renderType = 2;
                    }
                }
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: 32 * dp
                font.pixelSize: 16 * dp
                color: "#ffffff"
                text: "Context: <b>" + mainWindow.settingContextName + "</b>"
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: 32 * dp
                font.pixelSize: 16 * dp
                color: "#ffffff"
                text: "QNanoPainter backend: <b>" + mainWindow.settingQNanoBackendName + "</b>"
            }
            Text {
                anchors.right: parent.right
                anchors.rightMargin: 32 * dp
                font.pixelSize: 16 * dp
                color: "#ffffff"
                text: "QML Shape backend: <b>" + mainWindow.settingShapeBackendName + "</b>"
            }
            Item {
                width: 1
                height: 16 * dp
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

