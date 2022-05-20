cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter_examples VERSION 0.1.0)

set(CMAKE_AUTOGEN_PARALLEL AUTO)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)
find_package(Qt5Quick CONFIG REQUIRED)

#gallery TODO fix
# add_executable(gallery gallery/main.cpp  gallery/qml.qrc)
# target_link_libraries(gallery qnanopainter nanovg) 

# helloitem TODO fix 
# add_executable(helloitem helloitem/main.cpp  helloitem/qml.qrc)
# target_link_libraries(helloitem qnanopainter nanovg) 
# target_include_directories(helloitem PUBLIC shared)  

#hellowidget
add_executable(hellowidget hellowidget/main.cpp shared/shareddata.qrc)
target_link_libraries(hellowidget qnanopainter nanovg) 
target_include_directories(hellowidget PUBLIC shared)  

# hellowindow
add_executable(hellowindow hellowindow/main.cpp shared/shareddata.qrc)
target_link_libraries(hellowindow qnanopainter nanovg) 
target_include_directories(hellowindow PUBLIC shared)  
