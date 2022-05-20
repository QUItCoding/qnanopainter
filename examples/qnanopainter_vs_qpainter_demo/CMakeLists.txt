cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter_vs_qpainter_demo VERSION 0.1.0)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE) 

set(CMAKE_AUTOGEN_PARALLEL AUTO)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)
find_package(Qt5Quick CONFIG REQUIRED)



add_executable(qnanopainter_vs_qpainter_demo 
    main.cpp

    src/demoqnanoitem.cpp 
    src/demoqnanoitem.h

    src/demoqnanoitempainter.cpp 
    src/demoqnanoitempainter.h

    src/demoqpitem.cpp 
    src/demoqpitem.h

    qml.qrc
    ../../libqnanopainter/libqnanopainterdata.qrc
    
    )

target_link_libraries(qnanopainter_vs_qpainter_demo qnanopainter nanovg Qt5::Widgets Qt5::Quick) 
