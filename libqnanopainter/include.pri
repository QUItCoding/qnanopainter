
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

# Enable this to get drawing debug information
#DEFINES += QNANO_DEBUG

# Enable this to let Qt include OpenGL headers
DEFINES += QNANO_QT_GL_INCLUDE

# Test for Raspebery Pi GPU's proprietary Broadcomm Headers 
exists("/opt/vc/include") {
    message("libqnanopainter: defining QNANO_RASPBIAN_QUIRKS; QNANO_ENABLE_GLES3 unsupported on Raspbian ...")
    DEFINES += QNANO_RASPBIAN_QUIRKS
}
else {
    message("libqnanopainter: assuming platform supports GLES3, defining QNANO_ENABLE_GLES3 ...")
    # This will enable GLES3 (disable to force GLES2)
    DEFINES += QNANO_ENABLE_GLES3
}

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

# test for Raspebery Pi GPU's proprietary Broadcomm Headers 
exists("/opt/vc/include") { 
    message("libqnanopainter: building only GLES2 backend for Raspberry Pi...")
    CONFIG += build_raspi_backends
}
# Note: Due to Angle, windows might use either OpenGL (desktop) or
#       openGL ES (angle) backend.
android | ios | windows {
    message("libqnanopainter: Building GLES backends for 'android | ios | windows'...")
    CONFIG += build_gles_backends
}
!CONFIG(build_gles_backends):!CONFIG(build_raspi_backends) | windows:!wince {
    message("libqnanopainter: Building gl backends ...")
    CONFIG += build_gl_backends
}

CONFIG(build_raspi_backends) {
    message("Including QNanoPainter OpenGL ES2 backend for Raspberry Pi.")
HEADERS += \
    $$PWD/private/qnanobackendgles2.h
SOURCES +=  \
    $$PWD/private/qnanobackendgles2.cpp
}

CONFIG(build_gles_backends) {
    message("Including QNanoPainter OpenGL ES backends")
HEADERS += \
    $$PWD/private/qnanobackendgles2.h \
    $$PWD/private/qnanobackendgles3.h
SOURCES +=  \
    $$PWD/private/qnanobackendgles2.cpp \
    $$PWD/private/qnanobackendgles3.cpp
}

CONFIG(build_gl_backends) {
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
