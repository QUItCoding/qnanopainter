TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    src/galleryitem.cpp \
    src/galleryitempainter.cpp

HEADERS += \
    src/galleryitem.h \
    src/galleryitempainter.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Default rules for deployment.
include(deployment.pri)
