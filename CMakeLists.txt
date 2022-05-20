cmake_minimum_required(VERSION 3.0.0)
project(qnanopainter_master VERSION 0.1.0)


#add_library(qnanopainter_master qnanopainter_master.cpp)

add_subdirectory("libqnanopainter/nanovg") 
add_subdirectory("libqnanopainter") 
add_subdirectory("examples") 