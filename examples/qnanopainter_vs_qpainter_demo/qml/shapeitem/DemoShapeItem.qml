import QtQuick 2.9
import QtQuick.Shapes 1.0

Item {
    id: root

    property real animationTime: 0
    property int testCount: 1
    property int enabledTests: 0

    readonly property bool enableAntialiasing: mainWindow.settingAntialiasing

    // Setup colors
    readonly property color m_colorWhite: Qt.rgba(1, 1, 1, 1)
    readonly property color m_colorGray: Qt.rgba(180/255, 180/255, 180/255, 255)
    readonly property color m_colorBlack: Qt.rgba(0, 0, 0, 255)
    readonly property color m_color1: Qt.rgba(1, 1, 1, 150/255)
    readonly property color m_color2: Qt.rgba(180/255, 190/255, 40/255, 20/255)
    readonly property color m_color3: Qt.rgba(1, 1, 1, 80/255)

    // Note: When antialiasing is enabled enable layer & MSAA or double texture size
    // Tune these as suitable
    layer.enabled: enableAntialiasing
    layer.smooth: enableAntialiasing
    layer.samples: 4
    //layer.textureSize: Qt.size(root.width * 2, root.height * 2)

    FontLoader {
        id: m_testFont
        source: "../fonts/Roboto-Regular.ttf"
    }

    Shape {
        // Dummy Shape item to request used rendererType
        id: dummyShape
        vendorExtensionsEnabled: mainWindow.settingVendorExtensionsEnabled
        onRendererTypeChanged: {
            mainWindow.settingShapeBackendName =
                    (rendererType === 1) ? "GeometryRenderer"
                                         : (rendererType === 2) ? "NvprRenderer"
                                                                : (rendererType === 3) ? "SoftwareRenderer"
                                                                                       : "UnknownRenderer";
        }
    }

    Repeater {
        id: componentsRepeater
        model: root.testCount
        Item {
            readonly property real itemTime: root.animationTime + 0.3 * index
            anchors.fill: parent

            Item {
                id: rulerComponents
                anchors.fill: parent
                visible: enabledTests & 1
                RulerComponent {
                    width: parent.width
                    height: parent.height * 0.05
                    y: parent.height * 0.02
                    time: parent.visible ? itemTime : 0
                }
            }

            Item {
                id: circlesComponents
                readonly property real s: Math.min(parent.width, parent.height)
                readonly property real bigCircle: 80.0 + s*0.5
                readonly property real smallCircle: 40.0 + s*0.2
                anchors.fill: parent
                visible: enabledTests & 2

                CirclesComponent {
                    x: parent.width/2 - circlesComponents.bigCircle/2
                    y: parent.height * 0.1
                    width: circlesComponents.bigCircle
                    height: circlesComponents.bigCircle
                    time: parent.visible ? itemTime * 2 : 0
                    items: 10
                }
                CirclesComponent {
                    x: parent.width * 0.05
                    y: parent.height * 0.55
                    width: circlesComponents.smallCircle
                    height: circlesComponents.smallCircle
                    time: parent.visible ? itemTime * 3 : 0
                    items: 8
                }
                CirclesComponent {
                    x: parent.width - circlesComponents.smallCircle - parent.width * 0.05
                    y: parent.height * 0.55
                    width: circlesComponents.smallCircle
                    height: circlesComponents.smallCircle
                    time: parent.visible ? itemTime : 0
                    items: 3
                }
            }

            Item {
                id: linesComponents
                anchors.fill: parent
                visible: enabledTests & 4

                LinesComponent {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height
                    time: parent.visible ? itemTime : 0
                    items: 4
                }
                LinesComponent {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height*0.8
                    time: parent.visible ? itemTime+10 : 0
                    items: 6
                }
                LinesComponent {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height*0.6
                    time: parent.visible ? itemTime/2 : 0
                    items: 12
                }
            }

            Item {
                id: barsComponents
                anchors.fill: parent
                visible: enabledTests & 8
                BarsComponent {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height * 0.8
                    time: parent.visible ? itemTime * 3 : 0
                    items: 6
                }
                BarsComponent {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height * 0.4
                    time: parent.visible ? itemTime + 2 : 0
                    items: 10
                }
                BarsComponent {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height * 0.3
                    time: parent.visible ? itemTime * 2 + 2 : 0
                    items: 20
                }
                BarsComponent {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height * 0.2
                    time: parent.visible ? itemTime * 3 + 2 : 0
                    items: 40
                }
            }

            Item {
                id: iconsComponents
                anchors.fill: parent
                visible: enabledTests & 16
                IconsComponent {
                    y: parent.height * 0.2
                    width: parent.width
                    height: parent.height * 0.2
                    time: parent.visible ? itemTime : 0
                    items: 50
                }
            }

            Item {
                id: flowersComponents
                readonly property real s: Math.min(parent.width, parent.height)
                readonly property real flowerSize: 80.0 + s*0.6
                anchors.fill: parent
                visible: enabledTests & 32
                FlowerComponent {
                    x: parent.width/2 - flowersComponents.flowerSize/2
                    y: parent.height - flowersComponents.flowerSize
                    width: flowersComponents.flowerSize
                    height: flowersComponents.flowerSize
                    time: parent.visible ? itemTime : 0
                }
            }

        }
    }
}
