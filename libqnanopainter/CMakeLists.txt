cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)
find_package(Qt5Quick CONFIG REQUIRED)

add_library(qnanopainter 
    qnanoboxgradient.cpp qnanoboxgradient.h
    qnanocolor.cpp  qnanocolor.h
    qnanofont.cpp qnanofont.h
    qnanoimage.cpp qnanoimage.h
    qnanoimagepattern.cpp qnanoimagepattern.h
    qnanolineargradient.cpp qnanolineargradient.h
    qnanopainter.cpp qnanopainter.h
    qnanoquickitem.cpp qnanoquickitem.h
    qnanoquickitempainter.cpp qnanoquickitempainter.h
    qnanoradialgradient.cpp qnanoradialgradient.h
    qnanowidget.cpp qnanowidget.h
    qnanowindow.cpp qnanowindow.h

    private/qnanobackendgles2.h 
    private/qnanobackendgles3.h
    private/qnanobackendgles2.cpp 
    private/qnanobackendgles3.cpp
    private/qnanodebug.cpp
    private/qnanodebug.h
    
    private/qnanobackendgl3.cpp private/qnanobackendgl3.h
    private/qnanobackendgl2.cpp private/qnanobackendgl2.h
    )



    target_link_libraries(qnanopainter Qt5::Widgets Qt5::Quick) 

    target_include_directories(qnanopainter PUBLIC .)  

    target_compile_definitions(qnanopainter PUBLIC QNANO_QT_GL_INCLUDE)
    target_compile_definitions(qnanopainter PUBLIC QNANO_DEBUG)
    target_compile_definitions(qnanopainter PUBLIC QNANO_ENABLE_GLES3)
    target_compile_definitions(qnanopainter PUBLIC QNANO_BUILD_GL_BACKENDS)
    #target_compile_definitions(qnanopainter PUBLIC QNANO_BUILD_GLES_BACKENDS)

    
    

