
# Add this into include path
INCLUDEPATH += $$PWD/

# Use c++11 features
CONFIG += c++11

win32  {
    QT_CONFIG += opengles2 angle
    CONFIG( debug, debug|release ) {
        # debug
        LIBS+= -llibGLESV2d
    } else {
        # release
        LIBS+= -llibGLESV2
    }
}

# Enable this to get drawind debug information
#DEFINES += QNANO_DEBUG

# Enable this to let Qt include OpenGL headers
DEFINES += QNANO_QT_GL_INCLUDE

# This will enable GLES3 (disable to force GLES2)
DEFINES += QNANO_ENABLE_GLES3

equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 7) {
    message("Building with Qt at least 5.8 so may enabled QNANO_USE_RENDERNODE")
    # Enable this to use QRenderNode (available since Qt 5.8.0) instead of QQuickFramebufferObject
    #DEFINES += QNANO_USE_RENDERNODE
}

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
    $$PWD/qnanopainter.cpp \
    $$PWD/qnanocolor.cpp \
    $$PWD/qnanolineargradient.cpp \
    $$PWD/qnanoimagepattern.cpp \
    $$PWD/qnanoimage.cpp \
    $$PWD/qnanofont.cpp \
    $$PWD/qnanoradialgradient.cpp \
    $$PWD/qnanoboxgradient.cpp \
    $$PWD/qnanowindow.cpp \
    $$PWD/private/qnanodebug.cpp

HEADERS +=  \
    $$PWD/private/qnanobrush.h \
    $$PWD/qnanopainter.h \
    $$PWD/qnanocolor.h \
    $$PWD/qnanolineargradient.h \
    $$PWD/qnanoimagepattern.h \
    $$PWD/qnanoimage.h \
    $$PWD/qnanofont.h \
    $$PWD/qnanoradialgradient.h \
    $$PWD/qnanoboxgradient.h \
    $$PWD/private/qnanodataelement.h \
    $$PWD/private/qnanobackend.h \
    $$PWD/private/qnanobackendfactory.h \
    $$PWD/qnanowindow.h \
    $$PWD/private/qnanodebug.h

contains(QT, quick) {
    SOURCES +=  \
        $$PWD/qnanoquickitem.cpp \
        $$PWD/qnanoquickitempainter.cpp
    HEADERS +=  \
        $$PWD/qnanoquickitem.h \
        $$PWD/qnanoquickitempainter.h
}

contains(QT, widgets) {
    SOURCES +=  \
        $$PWD/qnanowidget.cpp
    HEADERS +=  \
        $$PWD/qnanowidget.h
}

# Note: Due to Angle, windows might use either OpenGL (desktop) or
#       openGL ES (angle) backend.
android | ios | windows {
    message("Including QNanoPainter OpenGL ES backends")
HEADERS += \
    $$PWD/private/qnanobackendgles2.h \
    $$PWD/private/qnanobackendgles3.h
SOURCES +=  \
    $$PWD/private/qnanobackendgles2.cpp \
    $$PWD/private/qnanobackendgles3.cpp
}

osx | linux:!android | windows {
    message("Including QNanoPainter OpenGL backends")
HEADERS += \
    $$PWD/private/qnanobackendgl2.h \
    $$PWD/private/qnanobackendgl3.h
SOURCES +=  \
    $$PWD/private/qnanobackendgl2.cpp \
    $$PWD/private/qnanobackendgl3.cpp
}

## Include NanoVG
SOURCES +=  $$PWD/nanovg/nanovg.c
HEADERS +=  $$PWD/nanovg/nanovg.h

RESOURCES += \
    $$PWD/libqnanopainterdata.qrc
