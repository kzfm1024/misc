cmake_minimum_required(VERSION 2.8)
project(slTriangle)
set(CMAKE_CXX_FLAGS "-g")
#include_directories(.)
add_executable(slTriangle slTriangle.cpp)
target_link_libraries(slTriangle GLEW glut GLU GL m)
