cmake_minimum_required(VERSION 2.8)
project(atomic_flag)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(atomic_flag atomic_flag.cpp)
target_link_libraries(atomic_flag pthread)
