import QtQuick 2.9
import QtQuick.Shapes 1.0

Item {
    id: root

    property real time: 0
    property int items: 100
    readonly property real space: width * 0.03 + Math.sin(time) * width * 0.02

    readonly property real h1: height * 0.2
    readonly property real h2: height * 0.3
    readonly property real h3: height * 0.5

    x: width * 0.05

    // Ruler labels
    Repeater {
        model: 20
        Text {
            x: space*5*index - paintedWidth/2
            y: root.height - paintedHeight/2
            font.pixelSize: 10.0 + root.width*0.01
            font.family: m_testFont.name
            color: "#E0E0B0"
            text: index * 5
            visible: (index%2==0) || (space > root.width*0.02)
        }
    }

    // Ruler lines
    Repeater {
        model: items
        Rectangle {
            x: space * index
            y: 0
            width: 1
            height: (index%10==0) ? h3 : (index%5==0) ? h2 : h1
            color: "#E0E0E0"
        }
    }

    /*

    // Alternative for ruler lines using QML Shape
    // Based on testing with Qt 5.10 beta 3, performanse of this is slower than with Rectangles

    Component.onCompleted: {
        var newObject = 'import QtQuick 2.9; import QtQuick.Shapes 1.0;';
        newObject += 'Shape {' +
        'readonly property real space: root.space;' +
        'readonly property real h1: root.h1;' +
        'readonly property real h2: root.h2;' +
        'readonly property real h3: root.h3;' +
        'readonly property real itemWidth: root.width;' +
        'readonly property real itemHeight: root.height;' +
        'asynchronous: true;' +
        'vendorExtensionsEnabled: mainWindow.settingVendorExtensionsEnabled;';
        newObject += 'ShapePath {' +
                'strokeColor: "#E0E0E0";' +
                'fillColor: "transparent";' +
                'strokeWidth: 1;';
        for (var i=0; i<items; ++i) {
            var lineHeight = (i%10==0) ? h3 : (i%5==0) ? h2 : h1;
            newObject += 'PathMove { x: space* ' + i + '; y: 0; }';
            newObject += 'PathLine { x: space* ' + i + '; y: ' + lineHeight + '; }';
        }
        newObject += '}'; // ShapePath
        newObject += '}'; // Shape

        var p = Qt.createQmlObject(newObject, root, "ruler_lines_component");
        shapeItem.data.push(p);
    }

    Item {
        id: shapeItem
        anchors.fill: parent
    }
*/

}
