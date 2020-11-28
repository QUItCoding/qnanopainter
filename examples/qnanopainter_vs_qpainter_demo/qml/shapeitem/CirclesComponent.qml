import QtQuick 2.9
import QtQuick.Shapes 1.0

Item {
    id: root

    property real time: 0
    property int items: 0

    readonly property real barWidth: 0.3 * width / items
    readonly property real lineMargin: 0.2 * barWidth
    readonly property real showAnimationProgress: 0.1 + 0.4*Math.sin(time * 0.8) + 0.5
    readonly property real lineWidth: barWidth * showAnimationProgress
    readonly property real cx: width/2
    readonly property real cy: height/2

    function ringProgress(i) {
        return showAnimationProgress * (2*Math.PI) * (1 - i/items) - Math.PI/2;
    }

    function useLargeArcFunc(i) {
        if (ringProgress(i) > Math.PI/2) {
            return true;
        } else {
            return false;
        }
    }

    onItemsChanged: {
        var newObject = 'import QtQuick 2.9; import QtQuick.Shapes 1.0;';
        newObject += 'Shape {' +
        'readonly property real lineWidth: root.lineWidth;' +
        'readonly property real lineSpacing: lineWidth + root.lineMargin;' +
        'readonly property real itemWidth: root.width;' +
        'readonly property real itemHeight: root.height;' +
        'asynchronous: true;' +
        'vendorExtensionsEnabled: mainWindow.settingVendorExtensionsEnabled;\n';

        for (var i=0; i<items; ++i) {
            newObject += 'ShapePath {' +
                    'readonly property real s: ' + i + '/items;' +
                    'strokeColor: Qt.rgba((200-150*s)/255, (200-50*s)/255, (100+50*s)/255, showAnimationProgress);' +
                    'fillColor: "transparent";' +
                    'strokeWidth: lineWidth;' +
                    'capStyle: ShapePath.RoundCap;';
            newObject += 'PathMove { x: itemWidth/2; y: ' + i + '*lineSpacing+lineWidth; }';
            newObject += 'PathArc { x: cx + ((itemWidth/2-' + i + '*lineSpacing-lineWidth) * Math.cos(ringProgress(' + i + '))); y: cy + ((itemWidth/2-' + i + '*lineSpacing-lineWidth) * Math.sin(ringProgress(' + i + '))); radiusX: itemWidth/2-' + i + '*lineSpacing-lineWidth; radiusY: itemWidth/2-' + i + '*lineSpacing-lineWidth; useLargeArc: useLargeArcFunc(' + i + '); }';
            newObject += '}\n'; // ShapePath
        }
        newObject += '}'; // Shape

        var p = Qt.createQmlObject(newObject, root, "circle_component");
        shapeItem.data.push(p);
    }

    Repeater {
        model: root.items
        Rectangle {
            // Note: QML anchoring rounds to closest pixel which causes jaggies,
            // so using x & y properties instead
            //anchors.centerIn: parent
            x: parent.width/2 - width/2
            y: parent.height/2 - height/2
            width: root.width - index * 2 * (lineWidth + lineMargin) - lineWidth
            height: width
            radius: width/2
            color: "transparent"
            border.color: Qt.rgba(215/255, 215/255, 215/255, 50/255)
            border.width: lineWidth
        }
    }

    Item {
        id: shapeItem
        anchors.fill: parent
    }
}
