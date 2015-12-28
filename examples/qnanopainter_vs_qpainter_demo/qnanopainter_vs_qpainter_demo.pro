TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    src/demoqpitem.cpp \
    src/demoqnanoitem.cpp \
    src/demoqnanoitempainter.cpp

HEADERS += \
    src/demoqpitem.h \
    src/demoqnanoitem.h \
    src/demoqnanoitempainter.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Default rules for deployment.
include(deployment.pri)
