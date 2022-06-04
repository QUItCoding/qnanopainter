cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter_vs_qpainter_demo VERSION 0.1.0)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE) 

set(CMAKE_AUTOGEN_PARALLEL AUTO)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
message(STATUS "DEMO QT VERSION=${QT_VERSION_MAJOR}" )
if(QT_VERSION_MAJOR EQUAL  6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL Quick OpenGLWidgets)
    set(QNANOPAINTER_QT_LIBS  Qt::Widgets Qt::OpenGL Qt::Quick Qt::OpenGLWidgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL Quick )
    set(QNANOPAINTER_QT_LIBS  Qt5::Widgets Qt5::OpenGL Qt5::Quick)
endif() 

add_executable(qnanopainter_vs_qpainter_demo 
    main.cpp

    src/demoqnanoitem.cpp 
    src/demoqnanoitem.h

    src/demoqnanoitempainter.cpp 
    src/demoqnanoitempainter.h

    src/demoqpitem.cpp 
    src/demoqpitem.h

    qml.qrc
)

target_link_libraries(qnanopainter_vs_qpainter_demo qnanopainter ${QNANOPAINTER_QT_LIBS} )
