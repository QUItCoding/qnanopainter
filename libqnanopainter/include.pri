
# Add this into include path
INCLUDEPATH += $$PWD/

# Use c++11 features
CONFIG += c++11

# Enable this to get drawing debug information
#DEFINES += QNANO_DEBUG

# Enable this to let Qt include OpenGL headers
DEFINES += QNANO_QT_GL_INCLUDE

# This will enable GLES3 (disable to force GLES2)
DEFINES += QNANO_ENABLE_GLES3

# This will enable signalling touch events
# Can be useful when using view/widget classes directly
#DEFINES += QNANO_ENABLE_TOUCH_SIGNALS

# This will enable signalling paint events
# Can be useful when using view/widget classes directly
#DEFINES += QNANO_ENABLE_PAINT_SIGNALS

equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 7) {
    message("Building with Qt at least 5.8 so may enabled QNANO_USE_RENDERNODE")
    # Enable this to use QRenderNode (available since Qt 5.8.0) instead of QQuickFramebufferObject
    #DEFINES += QNANO_USE_RENDERNODE
}

# Configure the amount of logging in release build
CONFIG(release, debug|release) {
    message("QNanoPainter Release build")
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
#       OpenGL ES (angle) backend. Newer Qt versions don't automatically
#       link with OpenGL ES libraries.
win32 {
    equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 14) {
        CONFIG += windows_with_gles
        QT_CONFIG += opengles2 angle
        CONFIG( debug, debug|release ) {
            # debug
            LIBS+= -llibGLESv2
        } else {
            # release
            LIBS+= -llibGLESV2
        }
    }
}

# Trying to autodetect suitable backeds for different environments.
# Alternatively, define these manually:
#    DEFINES += QNANO_BUILD_GLES_BACKENDS
#    DEFINES += QNANO_BUILD_GL_BACKENDS

android | ios | linux-rasp-* | windows:wince | CONFIG(windows_with_gles) {
    DEFINES += QNANO_BUILD_GLES_BACKENDS
}
!contains(DEFINES , QNANO_BUILD_GLES_BACKENDS) | windows:!wince {
    DEFINES += QNANO_BUILD_GL_BACKENDS
}

contains(DEFINES , QNANO_BUILD_GLES_BACKENDS) {
    message("Including QNanoPainter OpenGL ES backends")

HEADERS += \
    $$PWD/private/qnanobackendgles2.h \
    $$PWD/private/qnanobackendgles3.h
SOURCES +=  \
    $$PWD/private/qnanobackendgles2.cpp \
    $$PWD/private/qnanobackendgles3.cpp
}

contains(DEFINES , QNANO_BUILD_GL_BACKENDS) {
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
