cmake_minimum_required(VERSION 2.8)
project(glGouraud)
set(CMAKE_CXX_FLAGS "-g")
add_executable(glGouraud glGouraud.cpp)
target_link_libraries(glGouraud glut GLU GL m)
