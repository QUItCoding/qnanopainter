cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter VERSION 0.1.0)


#make sure MOC compiler is called in parallel
set(CMAKE_AUTOGEN_PARALLEL AUTO)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Instruct CMake to create code from Qt resource files (*.qrc)
set(CMAKE_AUTORCC ON)

# UI ==> H generator from QT   # Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)

find_package(Qt5Quick CONFIG REQUIRED)



# aux_source_directory(. SRC_FILES)
# aux_source_directory(data SRC_FILES) 
# aux_source_directory(logic SRC_FILES) 

# add_library(qnanopainter  STATIC ${SRC_FILES})

add_library(qnanopainter 
    libqnanopainterdata.qrc
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
    
    private/qnanobackendgl3.cpp private/qnanobackendgl3.h
    private/qnanobackendgl2.cpp private/qnanobackendgl2.h

    )



    target_link_libraries(qnanopainter Qt5::Widgets Qt5::Quick) 

    target_include_directories(qnanopainter PUBLIC .)  

    target_compile_definitions(qnanopainter PUBLIC QNANO_QT_GL_INCLUDE)
    #target_compile_definitions(qnanopainter PUBLIC QNANO_DEBUG)
    target_compile_definitions(qnanopainter PUBLIC QNANO_ENABLE_GLES3)
    

