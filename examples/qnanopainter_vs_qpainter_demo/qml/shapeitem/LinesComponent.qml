import QtQuick 2.9
import QtQuick.Shapes 1.0

Item {
    id: root

    property real time: 0
    property int items: 0

    readonly property real dx: width / (items-1)
    readonly property real dotSize: 4.0 + width * 0.005

    function posY(i) {
        return root.height - (0.5 + Math.sin((i+1) * time * 0.2) * 0.1) * 0.8 * root.height;
    }

    function posX(i) {
        return i * dx;
    }

    onItemsChanged: {
        var newObject = 'import QtQuick 2.9; import QtQuick.Shapes 1.0;';
        newObject += 'Shape {' +
        'readonly property real dx: root.dx;' +
        'readonly property real dotSize: root.dotSize;' +
        'readonly property real itemWidth: root.width;' +
        'readonly property real itemHeight: root.height;' +
        // Note: asynchronous disabled for now, see QTBUG-64508
        //'asynchronous: true;' +
        'vendorExtensionsEnabled: mainWindow.settingVendorExtensionsEnabled;\n';

        // Draw graph background area
        newObject += 'ShapePath {' +
                'strokeColor: "transparent";' +
                'fillGradient: LinearGradient {' +
                'y1:0; y2:itemHeight;' +
                'GradientStop { position: 0; color: m_color1 }' +
                'GradientStop { position: 1; color: m_color2 }' +
                '}' +
                'startX: posX(0);' +
                'startY: itemHeight;\n';
        newObject += 'PathLine { x: posX(0); y: posY(0); }\n';
        var i;
        for (i=1; i<items; ++i) {
            newObject += 'PathCubic { control1X: posX(' + (i-1) + ')+dx*0.5; control1Y: posY(' + (i-1) + '); control2X: posX(' + i + ')-dx*0.5; control2Y: posY(' + i + '); x: posX(' + i + '); y: posY(' + i + ') }\n';
        }
        newObject += 'PathLine { x: itemWidth; y: itemHeight; }';
        newObject += '}\n'; // ShapePath

        // Draw graph line
        newObject += 'ShapePath {' +
                'strokeColor: m_colorGray;' +
                'strokeWidth: 1.0 + dotSize*0.2;' +
                'fillColor: "transparent";' +
                'startX: posX(0);' +
                'startY: posY(0);\n';
        for (i=1; i<items; ++i) {
            newObject += 'PathCubic { control1X: posX(' + (i-1) + ')+dx*0.5; control1Y: posY(' + (i-1) + '); control2X: posX(' + i + ')-dx*0.5; control2Y: posY(' + i + '); x: posX(' + i + '); y: posY(' + i + ') }\n';
        }
        newObject += '}\n'; // ShapePath

        // Draw dots
        for (i=0; i<items; ++i) {
        newObject += 'Rectangle {' +
                'x: posX(' + i +') - width/2;' +
                'y: posY(' + i +') - height/2;' +
                'width: dotSize*2*0.9;' +
                'height: width;' +
                'radius: height/2;' +
                'color: m_colorWhite;' +
                'border.color: m_colorBlack;' +
                'border.width: dotSize*0.2; }\n';
        }

        /*
        // Alternative for dots using QML Shape
        newObject += 'ShapePath {' +
                'strokeColor: m_colorBlack;' +
                'fillColor: m_colorWhite;' +
                'strokeWidth: dotSize*0.2;';
        for (i=0; i<items; ++i) {
            // Drawing circle with 2 arcs
            newObject += 'PathMove { x: posX(' + i +') - dotSize*0.8; y: posY(' + i +'); }';
            newObject += 'PathArc { relativeX: dotSize*2*0.8; relativeY: 0; radiusX: dotSize*0.4; radiusY: dotSize*0.4; useLargeArc: true }';
            newObject += 'PathArc { relativeX: -dotSize*2*0.8; relativeY: 0; radiusX: dotSize*0.4; radiusY: dotSize*0.4; useLargeArc: true }';
        }
        newObject += '}'; // ShapePath
        */

        newObject += '}'; // Shape

        var p = Qt.createQmlObject(newObject, root, "bezier_line_component");
        shapeItem.data.push(p);
    }

    Item {
        id: shapeItem
        anchors.fill: parent
    }

}
