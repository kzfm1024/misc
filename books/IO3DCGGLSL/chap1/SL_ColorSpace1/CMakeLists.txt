cmake_minimum_required(VERSION 2.8)
project(glColorSpace)
set(CMAKE_CXX_FLAGS "-g")
#include_directories(.)
add_executable(slColorSpace1 slColorSpace1.cpp)
target_link_libraries(slColorSpace1 GLEW glut GLU GL m)
