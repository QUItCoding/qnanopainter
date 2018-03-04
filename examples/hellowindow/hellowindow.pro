TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    hellowindow.h

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Default rules for deployment.
include(deployment.pri)

