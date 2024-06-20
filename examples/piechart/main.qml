import QtQuick 2.4
import QtQuick.Window 2.2
import PieChart 1.0
import "."
Window {
    id: root
    property real dp: Screen.pixelDensity * 25.4/160
    // This will be 1 on most platforms, 2 on iOS double retina, 3 on iPhone6 plus
    property int dPRatio: Screen.devicePixelRatio

    visible: true
    width: Screen.primaryOrientation === Qt.LandscapeOrientation ? 800:480
    height: Screen.primaryOrientation === Qt.LandscapeOrientation ? 480:800
    color: "#404040"

    Text {
        id: textItem
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height * 0.01
        font.pixelSize: parent.width * 0.015
        color: "lightgray"
        text: "Here are some piecharts"
    }

    ListModel {
        id: pieModel1

        ListElement {
            color: "#99e600"
            value: 21
        }
        ListElement {
            color: "#99cc00"
            value: 18
        }
        ListElement {
            color: "#99b300"
            value: 14
        }
        ListElement {
            color: "#9f991a"
            value: 13
        }
        ListElement {
            color: "#a48033"
            value: 13
        }
        ListElement {
            color: "#a9664d"
            value: 13
        }
        ListElement {
            color: "ae4d66"
            value: 12
        }
        ListElement {
            color: "#b33380"
            value: 11
        }
        ListElement {
            color: "#a64086"
            value: 9
        }
        ListElement {
            color: "#994d8d"
            value: 9
        }
        ListElement {
            color: "#8d5a93"
            value: 7
        }
        ListElement {
            color: "806699"
            value: 6
        }
        ListElement {
            color: "#8073a6"
            value: 4
        }
        ListElement {
            color: "#8080b3"
            value:38
        }
    }
    ListModel {
        id: pieModel2

        ListElement {
            color: "black"
            value: 5
        }
        ListElement {
            color: "red"
            value: 15
        }
        ListElement {
            color: "blue"
            value: 10
        }
        ListElement {
            color: "gray"
            value: 15
        }
        ListElement {
            color: "green"
            value: 20
        }
        ListElement {
            color: "lightcyan"
            value: 25
        }
        ListElement {
            color: "lemonchiffon"
            value: 30
        }
        ListElement {
            color: "limegreen"
            value: 35
        }
        ListElement {
            color: "salmon"
            value: 15
        }
    }
    ListModel {
        id: pieModel3

        ListElement {
            color: "navy"
            value: 1
        }
        ListElement {
            color: "oldlace"
            value: 2
        }
        ListElement {
            color: "orchid"
            value: 3
        }
        ListElement {
            color: "royalblue"
            value: 4
        }
        ListElement {
            color: "firebrick"
            value: 5
        }
        ListElement {
            color: "darkseagreen"
            value: 6
        }
        ListElement {
            color: "cornsilk"
            value: 7
        }
        ListElement {
            color: "cadetblue"
            value: 8
        }
        ListElement {
            color: "burlywood"
            value: 9
        }
    }
    Flickable {
        anchors.top: textItem.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        contentWidth: grid.width; contentHeight: grid.height
    Grid {
        id: grid
        columns: width / 80
        spacing: 0
        width: root.width
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeWidth: 4
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1200
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    easing.type: Easing.Linear
                    duration: 1200
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 550
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 500
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 250
                    easing.type: Easing.InSine
                    to: 100
                }
                PauseAnimation {
                    duration: 100
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    duration: 1750
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeColor: "lightgray"
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true
            segmentShowStroke:false
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutSine
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeWidth:3
            segmentStrokeColor: "black"
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 250
                    easing.type: Easing.InSine
                    to: 100
                }
                PauseAnimation {
                    duration: 100
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false
            segmentShowStroke:false
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    duration: 1750
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeColor:"darkblue"
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true
            segmentShowStroke:false
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutSine
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 250
                    easing.type: Easing.InSine
                    to: 100
                }
                PauseAnimation {
                    duration: 100
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    duration: 1750
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true


            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutSine
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeWidth: 4
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1200
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    easing.type: Easing.Linear
                    duration: 1200
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 550
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 500
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 250
                    easing.type: Easing.InSine
                    to: 100
                }
                PauseAnimation {
                    duration: 100
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    duration: 1750
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeColor: "lightgray"
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true
            segmentShowStroke:false
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutSine
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeWidth:3
            segmentStrokeColor: "black"
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 250
                    easing.type: Easing.InSine
                    to: 100
                }
                PauseAnimation {
                    duration: 100
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false
            segmentShowStroke:false
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    duration: 1750
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true
            segmentStrokeColor:"darkblue"
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true
            segmentShowStroke:false
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutSine
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 250
                    easing.type: Easing.InSine
                    to: 100
                }
                PauseAnimation {
                    duration: 100
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 3000
                    easing.type: Easing.Linear
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    duration: 1750
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true
            segmentShowStroke:false
            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutSine
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 250
                    easing.type: Easing.InSine
                    to: 100
                }
                PauseAnimation {
                    duration: 100
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel3
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: true

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 750
                    easing.type: Easing.OutBounce
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
        PieChart {
            model: pieModel1
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: true
            animateScale: false

            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutElastic
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }

        PieChart {
            model: pieModel2
            width:(parent.width - 2*parent.spacing)/parent.columns
            height: width
            animation: true
            animateRotate: false
            animateScale: true


            SequentialAnimation on animationProgress {
                loops: Animation.Infinite
                PropertyAnimation {
                    duration: 1500
                    easing.type: Easing.OutSine
                    to: 100
                }
                PauseAnimation {
                    duration: 1000
                }
                PropertyAnimation {
                    to: 0
                }
            }
        }
    }
    }
    FpsItem {
        id: fpsItem
    }

}
