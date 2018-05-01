TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    hellowindow.h

# Include QNanoPainter
include(../../libqnanopainter/include.pri)

# Include shared examples content
include(../shared/sharedinclude.pri)

# Default rules for deployment.
include(deployment.pri)

