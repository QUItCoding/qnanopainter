cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter VERSION 0.1.0)

option(QNANO_DEBUG_COLLECT "Enable this to get the drawing debug information" OFF)
option(QNANO_DEBUG_RENDER "Enable this to render the drawing debug information" OFF)
option(QNANO_QT_GL_INCLUDE "Enable this to let Qt include OpenGL headers" ON)
option(QNANO_ENABLE_GLES3 "This will enable GLES3 (disable to force GLES2)" ON)
option(QNANO_ENABLE_TOUCH_SIGNALS "This will enable signalling touch events
                                   Can be useful when using view/widget classes directly" OFF)
option(QNANO_ENABLE_PAINT_SIGNALS "This will enable signalling paint events
                                   Can be useful when using view/widget classes directly" OFF)
option(QNANO_USE_RENDERNODE "Enable this to use QRenderNode (available since Qt 5.8.0) instead of QQuickFramebufferObject" OFF)
option(QNANO_ENABLE_PAINTER_SHARING "Enable this to share QNanoPainter instance between QNanoQuickItemPainters" ON)
option(QNANO_USE_RHI "Enable this to use QRhi (available since Qt 6.7) instead of QQuickFramebufferObject" OFF)

option(QNANO_BUILD_GLES_BACKENDS "When building for embedded devices you can define manually which backends are supported" OFF)
option(QNANO_BUILD_GL_BACKENDS "When building for embedded devices you can define manually which backends are supported" OFF)

# Qt6 from msys2 need extra lib
option(QNANO_MSYS2 "Enable this to build in MSYS2" OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    message(STATUS "QNANOPAINTER QT VERSION=${QT_VERSION_MAJOR}") 
if(QT_VERSION_MAJOR EQUAL  6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL Quick OpenGLWidgets Gui)
    set(QNANOPAINTER_QT_LIBS  Qt6::Widgets Qt6::OpenGL Qt6::Quick Qt6::OpenGLWidgets Qt::GuiPrivate)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL Quick )
    set(QNANOPAINTER_QT_LIBS  Qt5::Widgets Qt5::OpenGL Qt5::Quick)
endif() 


########################################### 
if(QT_VERSION_MAJOR EQUAL  5)
    set(IS_QUICK_IN_QT ON)
    set(IS_WIDGETS_IN_QT ON)
    # ????? set(QNANO_BUILD_GLES_BACKENDS ON)
    set(QNANO_BUILD_GL_BACKENDS ON)
else()
    if(QT_FEATURE_quick_canvas) # QT_FEATURE_quick not exists ??
        set(IS_QUICK_IN_QT ON)
    endif()    

    if(QT_FEATURE_widgets)
        set(IS_WIDGETS_IN_QT ON)
    endif()    
    
    if(QT_FEATURE_opengles2 OR QT_FEATURE_opengles3)
        set(QNANO_BUILD_GLES_BACKENDS ON)
    else()   
        set(QNANO_BUILD_GL_BACKENDS ON)
    endif()    
endif()

set(SOURCES 
    qnanopainter.cpp 
    qnanocolor.cpp 
    qnanolineargradient.cpp 
    qnanoimagepattern.cpp 
    qnanoimage.cpp 
    qnanofont.cpp 
    qnanoradialgradient.cpp 
    qnanoboxgradient.cpp 
    qnanowindow.cpp 
    private/qnanodebug.cpp
)

set(HEADERS 
    private/qnanobrush.h 
    qnanopainter.h 
    qnanocolor.h 
    qnanolineargradient.h 
    qnanoimagepattern.h 
    qnanoimage.h 
    qnanofont.h 
    qnanoradialgradient.h 
    qnanoboxgradient.h 
    private/qnanodataelement.h 
    private/qnanobackend.h 
    private/qnanobackendfactory.h 
    qnanowindow.h 
    private/qnanodebug.h
)


if(IS_QUICK_IN_QT)
    set(SOURCES ${SOURCES}
        qnanoquickitem.cpp 
        qnanoquickitempainter.cpp        
    )
    set(HEADERS ${HEADERS}
        qnanoquickitem.h 
        qnanoquickitempainter.h
    )
endif()

if(IS_WIDGETS_IN_QT)
    set(SOURCES ${SOURCES}
        qnanowidget.cpp      
    )
    set(HEADERS ${HEADERS}
        qnanowidget.h
    )
endif()

if(QNANO_USE_RHI)
    # Use RHI backend instead of OpenGL (ES) backends.
    set(QNANO_BUILD_GLES_BACKENDS OFF)
    set(QNANO_BUILD_GL_BACKENDS OFF)
    set(SOURCES ${SOURCES}
        private/qnanobackendrhi.cpp
        nanovg/nanovg_rhi.cpp
    )
    set(HEADERS ${HEADERS}
        private/qnanobackendrhi.h
        nanovg/nanovg_rhi.h
    )
endif()

if(QNANO_BUILD_GLES_BACKENDS)
    set(SOURCES ${SOURCES}
        private/qnanobackendgles2.h 
        private/qnanobackendgles3.h
    )
    set(HEADERS ${HEADERS}
        private/qnanobackendgles2.cpp 
        private/qnanobackendgles3.cpp
    )    
endif()

if(QNANO_BUILD_GL_BACKENDS)
    set(SOURCES ${SOURCES}
        private/qnanobackendgl2.h 
        private/qnanobackendgl3.h
    )
    set(HEADERS ${HEADERS}
        private/qnanobackendgl2.cpp 
        private/qnanobackendgl3.cpp
    )    
endif()

add_library(qnanopainter 
    ${SOURCES} 
    ${HEADERS}
    libqnanopainterdata.qrc
    nanovg/nanovg.c
    nanovg/nanovg.h
)

if (WIN32)
    if(QNANO_MSYS2)
        message(STATUS "BUILD MINGW add:GLESv2")
        set(QNANOPAINTER_QT_LIBS ${QNANOPAINTER_QT_LIBS} GLESv2)
    endif()
endif()

if(MSVC)
    # Suppress fontstash warnings about fopen & strncpy usage
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# add QT Libs 
target_link_libraries(qnanopainter ${QNANOPAINTER_QT_LIBS} )

# add to INCLUDE PATH 
target_include_directories(qnanopainter
    PUBLIC .
    PUBLIC nanovg)

# cmake variable ==> c++ #define
if(QNANO_QT_GL_INCLUDE)
    target_compile_definitions(qnanopainter PUBLIC QNANO_QT_GL_INCLUDE)
    message(STATUS "QNANO_QT_GL_INCLUDE=${QNANO_QT_GL_INCLUDE}")
endif() 
if(QNANO_DEBUG_COLLECT)
    target_compile_definitions(qnanopainter PUBLIC QNANO_DEBUG_COLLECT)
    message(STATUS "QNANO_DEBUG_COLLECT=${QNANO_DEBUG_COLLECT}")
endif()
if(QNANO_DEBUG_RENDER)
    target_compile_definitions(qnanopainter PUBLIC QNANO_DEBUG_RENDER)
    message(STATUS "QNANO_DEBUG_RENDER=${QNANO_DEBUG_RENDER}")
endif()
if(QNANO_BUILD_GL_BACKENDS)
    target_compile_definitions(qnanopainter PUBLIC QNANO_BUILD_GL_BACKENDS)
    message(STATUS "QNANO_BUILD_GL_BACKENDS=${QNANO_BUILD_GL_BACKENDS}")
endif()    
if(QNANO_BUILD_GLES_BACKENDS)
    target_compile_definitions(qnanopainter PUBLIC QNANO_BUILD_GLES_BACKENDS)
    message(STATUS "QNANO_BUILD_GLES_BACKENDS=${QNANO_BUILD_GLES_BACKENDS}")
endif() 
if(QNANO_ENABLE_GLES3)    
    target_compile_definitions(qnanopainter PUBLIC QNANO_ENABLE_GLES3)
    message(STATUS "QNANO_ENABLE_GLES3=${QNANO_ENABLE_GLES3}")
endif()    
if(QNANO_USE_RHI)
    target_compile_definitions(qnanopainter PUBLIC QNANO_USE_RHI)
    message(STATUS "QNANO_USE_RHI=${QNANO_USE_RHI}")
endif()
