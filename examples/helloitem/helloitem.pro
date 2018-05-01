TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Include shared examples content
include(../shared/sharedinclude.pri)

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    helloitem.h
