TEMPLATE = app

QT += widgets

SOURCES += main.cpp

HEADERS += \
    hellowidget.h

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Include shared examples content
include(../shared/sharedinclude.pri)

# Default rules for deployment.
include(deployment.pri)

