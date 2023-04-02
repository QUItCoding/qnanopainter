cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter_examples VERSION 0.1.0)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE) 

set(CMAKE_AUTOGEN_PARALLEL AUTO)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
if(QT_VERSION_MAJOR EQUAL  6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL Quick OpenGLWidgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL Quick )
endif() 

# gallery
add_executable(gallery 
    gallery/main.cpp  
    gallery/qml.qrc

    gallery/painting/src/paintingitem.cpp          gallery/painting/src/paintingitem.h
    gallery/painting/src/paintingitempainter.cpp   gallery/painting/src/paintingitempainter.h
    gallery/painting/painting.qrc

    gallery/mouse_events/src/eventitem.cpp   gallery/mouse_events/src/eventitem.h
    gallery/mouse_events/src/eventitempainter.cpp   gallery/mouse_events/src/eventitempainter.h
    gallery/mouse_events/mouse_events.qrc        

    gallery/qnanopainter_features/src/galleryitem.cpp  gallery/qnanopainter_features/src/galleryitem.h 
    gallery/qnanopainter_features/src/galleryitempainter.cpp  gallery/qnanopainter_features/src/galleryitempainter.h 
    gallery/qnanopainter_features/qnanopainter_features.qrc
)
target_link_libraries(gallery qnanopainter) 

# helloitem TODO fix 
add_executable(helloitem helloitem/main.cpp  helloitem/helloitem.h helloitem/qml.qrc shared/shareddata.qrc )
target_link_libraries(helloitem qnanopainter) 
target_include_directories(helloitem PUBLIC shared)  

#hellowidget
add_executable(hellowidget hellowidget/main.cpp shared/shareddata.qrc  )
target_link_libraries(hellowidget qnanopainter) 
target_include_directories(hellowidget PUBLIC shared)  

# hellowindow
add_executable(hellowindow hellowindow/main.cpp shared/shareddata.qrc )
target_link_libraries(hellowindow qnanopainter) 
target_include_directories(hellowindow PUBLIC shared)  

add_subdirectory("qnanopainter_vs_qpainter_demo") 

