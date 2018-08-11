cmake_minimum_required(VERSION 2.8)
project(new_handler)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(new_handler new_handler.cpp)
target_link_libraries(new_handler pthread)
