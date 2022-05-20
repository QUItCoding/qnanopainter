cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter_master VERSION 0.1.0)


if(WIN32)
else()
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")
endif()


add_subdirectory("libqnanopainter/nanovg") 
add_subdirectory("libqnanopainter") 
add_subdirectory("examples") 