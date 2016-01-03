import QtQuick 2.4
import QtQuick.Window 2.2
import PieChart 1.0
import "."
Window {
    property real dp: Screen.pixelDensity * 25.4/160
    // This will be 1 on most platforms, 2 on iOS double retina, 3 on iPhone6 plus
    property int dPRatio: Screen.devicePixelRatio

    visible: true
    width: 800
    height: 480
    color: "#404040"

    Text {
        id: textItem
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height * 0.02
        font.pixelSize: parent.width * 0.04
        color: "lightgray"
        text: "Here are some piecharts"
    }

    Grid {
        anchors.top: textItem.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        columns: 7
        spacing: 2

        PieChart {
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
    FpsItem {
        id: fpsItem
    }

}
