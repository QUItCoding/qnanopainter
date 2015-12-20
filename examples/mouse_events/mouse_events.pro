TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    eventitem.cpp \
    eventitempainter.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    eventitem.h \
    eventitempainter.h
