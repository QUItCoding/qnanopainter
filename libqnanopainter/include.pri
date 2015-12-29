
# Add this into include path
INCLUDEPATH += $$PWD/

# Use c++11 features
CONFIG += c++11

win32  {
    QT_CONFIG += opengles2 angle
}

# Enable this to get drawind debug information
#DEFINES += QNANO_DEBUG

# Configure the amount of logging in release build
CONFIG(release, debug|release) {
    message("QNanoPainter Relase build")
    #DEFINES += QT_NO_WARNING_OUTPUT
    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += QT_NO_INFO_OUTPUT
}

# Configure the amount of logging in debug build
CONFIG(debug, debug|release) {
    message("QNanoPainter Debug build")
    #DEFINES += QT_NO_WARNING_OUTPUT
    #DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += QT_NO_INFO_OUTPUT
}

SOURCES +=  \
    $$PWD/private/qnanofborenderer.cpp \
    $$PWD/qnanoquickitem.cpp \
    $$PWD/qnanoquickitempainter.cpp \
    $$PWD/qnanopainter.cpp \
    $$PWD/qnanocolor.cpp \
    $$PWD/qnanolineargradient.cpp \
    $$PWD/qnanoimagepattern.cpp \
    $$PWD/qnanoimage.cpp \
    $$PWD/qnanofont.cpp \
    $$PWD/qnanoradialgradient.cpp \
    $$PWD/qnanoboxgradient.cpp

HEADERS +=  \
    $$PWD/private/qnanofborenderer.h \
    $$PWD/private/qnanobrush.h \
    $$PWD/qnanoquickitem.h \
    $$PWD/qnanoquickitempainter.h \
    $$PWD/qnanopainter.h \
    $$PWD/qnanocolor.h \
    $$PWD/qnanolineargradient.h \
    $$PWD/qnanoimagepattern.h \
    $$PWD/qnanoimage.h \
    $$PWD/qnanofont.h \
    $$PWD/qnanoradialgradient.h \
    $$PWD/qnanoboxgradient.h \
    $$PWD/private/qnanodataelement.h

## Include NanoVG
SOURCES +=  $$PWD/nanovg/nanovg.c
HEADERS +=  $$PWD/nanovg/nanovg.h

RESOURCES += \
    $$PWD/libqnanopainterdata.qrc
