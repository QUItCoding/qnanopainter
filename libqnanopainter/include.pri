
# Add this into include path
INCLUDEPATH += $$PWD/

# Use c++11 features
CONFIG += c++11

# Enable this to get the drawing debug information
# This is available in QNanoQuickItem data property.
#DEFINES += QNANO_DEBUG_COLLECT

# Enable this to show the drawing debug information.
# This is rendered at the bottom of each item.
#DEFINES += QNANO_DEBUG_RENDER

# Enable this to let Qt include OpenGL headers
DEFINES += QNANO_QT_GL_INCLUDE

# Enable this to use QRhi (available since Qt 6.7) instead of QQuickFramebufferObject
#DEFINES += QNANO_USE_RHI

# This will enable GLES3 (disable to force GLES2)
DEFINES += QNANO_ENABLE_GLES3

# This will enable signalling touch events
# Can be useful when using view/widget classes directly
#DEFINES += QNANO_ENABLE_TOUCH_SIGNALS

# This will enable signalling paint events
# Can be useful when using view/widget classes directly
#DEFINES += QNANO_ENABLE_PAINT_SIGNALS

# Enable this to use QRenderNode (available since Qt 5.8.0) instead of QQuickFramebufferObject
#DEFINES += QNANO_USE_RENDERNODE

# Suppress fontstash warnings about fopen & strncpy usage
DEFINES += _CRT_SECURE_NO_WARNINGS

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

contains(DEFINES, QNANO_USE_RHI) {
    # Use RHI backend instead of OpenGL (ES) backends.
    message("Including QNanoPainter RHI backend")
    DEFINES -= QNANO_BUILD_GL_BACKENDS
    DEFINES -= QNANO_BUILD_GLES_BACKENDS

    SOURCES +=  \
        $$PWD/private/qnanobackendrhi.cpp \
        $$PWD/nanovg/nanovg_rhi.cpp

    HEADERS +=  \
        $$PWD/private/qnanobackendrhi.h \
        $$PWD/nanovg/nanovg_rhi.h
}

equals(QT_MAJOR_VERSION, 6) { ## for Qt6 need opengl module
    QT += opengl              ## see https://doc-snapshots.qt.io/qt6-dev/qtopengl-index.html
}                             ## NB: "Warning: This module should not be used anymore for new code. Please use the corresponding OpenGL classes in Qt GUI." ( https://doc-snapshots.qt.io/qt6-dev/qtgui-index.html#opengl-and-opengl-es-integration )

!contains(DEFINES, QNANO_USE_RHI) {

    # Note: Due to Angle, windows might use either OpenGL (desktop) or
    #       OpenGL ES (angle) backend. Newer Qt versions don't automatically
    #       link with OpenGL ES libraries.
    win32 {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 14) {
            CONFIG += windows_with_gles
            QT_CONFIG += opengles2 angle
            CONFIG( debug, debug|release ) {
                # debug
                LIBS+= -llibGLESv2d
            } else {
                # release
                LIBS+= -llibGLESV2
            }
        }
    }

    # When building for embedded devices you can define manually which
    # backends are supported
    #DEFINES += QNANO_BUILD_GLES_BACKENDS
    #DEFINES += QNANO_BUILD_GL_BACKENDS

    # Alternatively, trying to autodetect suitable backeds based on Qt configuration
    !contains(DEFINES, QNANO_BUILD_GL_BACKENDS) : !contains(DEFINES, QNANO_BUILD_GLES_BACKENDS) {
        equals(QT_ARCH,"wasm") {
            # WebAssembly uses GLES
            DEFINES += QNANO_BUILD_GLES_BACKENDS
        }
    }

    !contains(DEFINES, QNANO_BUILD_GL_BACKENDS) : !contains(DEFINES, QNANO_BUILD_GLES_BACKENDS) {
        qtConfig(opengles2) | qtConfig(opengles3) {
            DEFINES += QNANO_BUILD_GLES_BACKENDS
        } else {
            DEFINES += QNANO_BUILD_GL_BACKENDS
        }
        CONFIG(windows_with_gles) {
            DEFINES += QNANO_BUILD_GLES_BACKENDS
        }
    }

    # Or finally selecting suitable backeds for different environments.
    !contains(DEFINES, QNANO_BUILD_GL_BACKENDS) : !contains(DEFINES, QNANO_BUILD_GLES_BACKENDS) {
        android | ios | linux-rasp-* | windows:wince | CONFIG(windows_with_gles) {
            DEFINES += QNANO_BUILD_GLES_BACKENDS
        }
        !contains(DEFINES , QNANO_BUILD_GLES_BACKENDS) | windows:!wince {
            DEFINES += QNANO_BUILD_GL_BACKENDS
        }
    }

    !contains(DEFINES, QNANO_BUILD_GL_BACKENDS) : !contains(DEFINES, QNANO_BUILD_GLES_BACKENDS) {
        error("No QNanoPainter backends defined. Please modify libqnanopainter/include.pri")
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

} #QNANO_USE_RHI

## Include NanoVG
SOURCES +=  $$PWD/nanovg/nanovg.c
HEADERS +=  $$PWD/nanovg/nanovg.h

RESOURCES += \
    $$PWD/libqnanopainterdata.qrc
