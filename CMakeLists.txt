cmake_minimum_required(VERSION 3.15)
project(WBCpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(WBCpp main.cpp figures.h funcs.h funcs.cpp buttons.h)

target_link_libraries(WBCpp GL glut SOIL png )
