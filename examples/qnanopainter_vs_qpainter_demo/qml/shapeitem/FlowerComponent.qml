import QtQuick 2.9
import QtQuick.Shapes 1.0

Shape {
    id: root

    property real time: 0

    readonly property int items: 12

    readonly property real cx: width/2
    readonly property real cy: height/2

    function ringProgress(i) {
        return (2*Math.PI) * (1 - i/items) - Math.PI/2;
    }

    // Note: enabling asynchronous causes bottom Rectangle to sometimes appear
    // under ShapePath when "Render count" > 1.
    //asynchronous: true
    vendorExtensionsEnabled: mainWindow.settingVendorExtensionsEnabled

    rotation: Math.sin(time)*20

    ShapePath {
        strokeColor: "#40000000"
        strokeWidth: 4

        fillGradient: RadialGradient {
            centerX: cx; centerY: cy
            centerRadius: cx
            focalX: centerX; focalY: centerY
            GradientStop { position: 0; color: Qt.rgba(0.5 + Math.sin(time*2)*0.5, 0, 0.5 + Math.sin(time+Math.PI)*0.5, 1) }
            GradientStop { position: 1; color: "#ffffff" }
        }

        startX: cx
        startY: cy

        PathQuad { x: cx + cx * Math.cos(ringProgress(1)); y: cy + cy *Math.sin(ringProgress(1)); controlX: cx + cx * Math.cos(ringProgress(0)); controlY: cy + cy *Math.sin(ringProgress(0)) }
        PathQuad { x: cx; y: cy; controlX: cx + cx * Math.cos(ringProgress(2)); controlY: cy + cy *Math.sin(ringProgress(2)) }

        PathQuad { x: cx + cx * Math.cos(ringProgress(3)); y: cy + cy *Math.sin(ringProgress(3)); controlX: cx + cx * Math.cos(ringProgress(2)); controlY: cy + cy *Math.sin(ringProgress(2)) }
        PathQuad { x: cx; y: cy; controlX: cx + cx * Math.cos(ringProgress(4)); controlY: cy + cy *Math.sin(ringProgress(4)) }

        PathQuad { x: cx + cx * Math.cos(ringProgress(5)); y: cy + cy *Math.sin(ringProgress(5)); controlX: cx + cx * Math.cos(ringProgress(4)); controlY: cy + cy *Math.sin(ringProgress(4)) }
        PathQuad { x: cx; y: cy; controlX: cx + cx * Math.cos(ringProgress(6)); controlY: cy + cy *Math.sin(ringProgress(6)) }

        PathQuad { x: cx + cx * Math.cos(ringProgress(7)); y: cy + cy *Math.sin(ringProgress(7)); controlX: cx + cx * Math.cos(ringProgress(6)); controlY: cy + cy *Math.sin(ringProgress(6)) }
        PathQuad { x: cx; y: cy; controlX: cx + cx * Math.cos(ringProgress(8)); controlY: cy + cy *Math.sin(ringProgress(8)) }

        PathQuad { x: cx + cx * Math.cos(ringProgress(9)); y: cy + cy *Math.sin(ringProgress(9)); controlX: cx + cx * Math.cos(ringProgress(8)); controlY: cy + cy *Math.sin(ringProgress(8)) }
        PathQuad { x: cx; y: cy; controlX: cx + cx * Math.cos(ringProgress(10)); controlY: cy + cy *Math.sin(ringProgress(10)) }

        PathQuad { x: cx + cx * Math.cos(ringProgress(11)); y: cy + cy *Math.sin(ringProgress(11)); controlX: cx + cx * Math.cos(ringProgress(10)); controlY: cy + cy *Math.sin(ringProgress(10)) }
        PathQuad { x: cx; y: cy; controlX: cx + cx * Math.cos(ringProgress(12)); controlY: cy + cy *Math.sin(ringProgress(12)) }
    }

    Rectangle {
        anchors.centerIn: parent
        width: root.width*0.2
        height: width
        radius: width/2
        color: "#ffffff"
    }
}
