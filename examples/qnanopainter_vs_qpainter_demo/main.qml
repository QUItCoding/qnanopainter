import QtQuick 2.4
import QtQuick.Window 2.2
import DemoQPItem 1.0
import DemoQNanoItem 1.0
import "qml"

Window {
    id: mainWindow

    property real animationTime: 0
    // Cutting corners a bit and not using enum
    // 1=ruler, 2=circles, 4=lines, 8=bars, 16=icons, 32=flower
    property int enabledTests: 31
    // Amount of items (QNanoQuickItemPainter / QQuickPaintedItem / Shape Item) to render
    property int itemCount: 1
    // How many times selected tests are rendered per item
    property int testCount: 1
    property bool fullScreen: true
    // 0 = QNanoPainter, 1 = QQuickPaintedItem, 2 = QML Shape
    property int renderType: 0
    // Resolution-independent dp
    readonly property real dp: Screen.pixelDensity * 25.4/160
    // This will be 1 on most platforms, 2 on iOS double retina, 3 on iPhone6 plus
    property int dPRatio: Screen.devicePixelRatio

    property bool settingAntialiasing: true
    property bool settingPixelAlign: false
    property bool settingPixelAlignText: false
    property bool settingHighQualityRendering: false
    property bool settingFBORendering: false
    property bool settingAnimateSize: false
    property bool settingVendorExtensionsEnabled: true
    property string settingContextName: "(unknown)"
    property string settingQNanoBackendName: "(unknown)"
    property string settingShapeBackendName: "(unknown)"

    property real itemHeight: graphContainer.height

    width: 375
    height: 667
    visible: true
    minimumWidth: 400*dp

    color: "#404040"

    NumberAnimation on animationTime {
        id: animationTimeAnimation
        from: 0
        to: 360
        duration: 1000*360
        loops: Animation.Infinite
    }

    Item {
        id: graphContainer
        width: mainWindow.fullScreen ? parent.width : 256
        height: mainWindow.fullScreen ? parent.height - fpsItem.height : 256
        y: mainWindow.fullScreen ? fpsItem.height : (parent.height - height)/2
        x: mainWindow.fullScreen ? 0 : (parent.width - width)/2

        Repeater {
            id: itemRepeater
            model: mainWindow.itemCount

            Item {
                id: sizeAnimatedContainer
                readonly property real itemMargin: (mainWindow.width / (40 + mainWindow.itemCount*2))
                readonly property real itemPos: (index * itemMargin)
                readonly property real itemPosCentered: (index * itemMargin) - (itemRepeater.count/2 * itemMargin)
                // Move each item slightly to see the item amount
                x: mainWindow.fullScreen ? itemPos : itemPosCentered
                y: mainWindow.fullScreen ? itemPos : itemPosCentered
                width: parent.width
                height: mainWindow.itemHeight

                DemoQNanoItem {
                    id: qnItem
                    width: parent.width
                    height: visible ? parent.height : 0
                    enabledTests: mainWindow.enabledTests
                    testCount: mainWindow.testCount
                    animationTime: visible ? mainWindow.animationTime : 0
                    visible: renderType === 0
                    //fillColor: Qt.rgba(0.2, 0.4, 0.5, 1.0)
                    antialiasing: mainWindow.settingAntialiasing
                    pixelAlign: mainWindow.settingPixelAlign ? DemoQNanoItem.PixelAlignHalf : DemoQNanoItem.PixelAlignNone
                    pixelAlignText: mainWindow.settingPixelAlignText ? DemoQNanoItem.PixelAlignFull : DemoQNanoItem.PixelAlignNone
                    highQualityRendering: mainWindow.settingHighQualityRendering
                    onContextNameChanged: {
                        mainWindow.settingContextName = qnItem.contextName;
                    }
                    onBackendNameChanged: {
                        mainWindow.settingQNanoBackendName = qnItem.backendName;
                    }
                }

                DemoQPItem {
                    id: qpItem
                    // QQuickPaintedItem < Qt 5.6 doesn't support iOS retina resolutions (see QTBUG-40489).
                    // When running with older Qt and OSX/iOS, enable these instead to make drawing sharp.
                    /*width: parent.width * dPRatio
                    height: visible ? parent.height * dPRatio : 0
                    scale: 1 / dPRatio
                    transformOrigin: Item.TopLeft*/
                    width: parent.width
                    height: visible ? parent.height : 0
                    enabledTests: mainWindow.enabledTests
                    testCount: mainWindow.testCount
                    animationTime: visible ? mainWindow.animationTime : 0
                    visible: renderType === 1
                    qpAntialiasing: mainWindow.settingAntialiasing
                    qpRenderTargetFBO: mainWindow.settingFBORendering
                }
                Loader {
                    // Note: Using Loader so that this QML Shape demo is only
                    // enabled when running with Qt >= 5.10
                    id: shapeItemLoader
                    width: parent.width
                    height: visible ? parent.height : 0
                    visible: renderType === 2
                    enabled: gEnableShapeDemo && visible
                    source: enabled  ? "qml/shapeitem/DemoShapeItem.qml" : ""
                    Binding {
                        target: shapeItemLoader.item
                        property: "enabledTests"
                        value: mainWindow.enabledTests
                        when: shapeItemLoader.status == Loader.Ready
                    }
                    Binding {
                        target: shapeItemLoader.item
                        property: "testCount"
                        value: mainWindow.testCount
                        when: shapeItemLoader.status == Loader.Ready
                    }
                    Binding {
                        target: shapeItemLoader.item
                        property: "animationTime"
                        value: shapeItemLoader.visible ? mainWindow.animationTime : 0
                        when: shapeItemLoader.status == Loader.Ready
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            animationTimeAnimation.paused = !animationTimeAnimation.paused
        }
    }

    SettingsView {
        id: settigsView
    }

    FpsItem {
        id: fpsItem
    }

    Button {
        anchors.verticalCenter: fpsItem.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 60 * dp
        text: renderType === 0 ? "QNanoPainter" : renderType === 1 ?  "QPainter" : "QML Shape"
        onClicked: {
            var availableDemos = gEnableShapeDemo ? 3 : 2
            if (renderType < (availableDemos-1)) {
                renderType++;
            } else {
                renderType = 0;
            }
        }
    }

    SequentialAnimation {
        id: sizeAnimation
        loops: Animation.Infinite
        running: mainWindow.settingAnimateSize
        NumberAnimation  {
            target: mainWindow
            property: "itemHeight"
            from: graphContainer.height
            to: graphContainer.height * 0.5
            duration: 1000
            easing.type: Easing.InOutQuad
        }
        NumberAnimation  {
            target: mainWindow
            property: "itemHeight"
            from: graphContainer.height * 0.5
            to: graphContainer.height
            duration: 1000
            easing.type: Easing.InOutQuad
        }
    }
}
