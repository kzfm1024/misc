cmake_minimum_required(VERSION 2.8)
project(slShadowMap1)
set(CMAKE_CXX_FLAGS "-g")
include_directories(.)
add_executable(slShadowMap1 slShadowMap1.cpp)
target_link_libraries(slShadowMap1 GLEW glut GLU GL m)
