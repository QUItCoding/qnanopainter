TEMPLATE = app

QT += widgets

equals(QT_MAJOR_VERSION, 6) { ## for Qt6 need openglwidgets module to define QOpenGLWidget
    QT += openglwidgets       ## see https://doc-snapshots.qt.io/qt6-dev/sourcebreaks.html#changes-to-qt-ope
}                             ## NB: "QOpenGLWidget has been moved to its own openglwidgets module. Applications relying on QOpenGLWidget should add QT += openglwidgets to their project file."

SOURCES += main.cpp

HEADERS += \
    hellowidget.h

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Include shared examples content
include(../shared/sharedinclude.pri)

# Default rules for deployment.
include(deployment.pri)

