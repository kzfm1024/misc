cmake_minimum_required(VERSION 2.8)
project(c++exception)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(exception_test exception_test.cpp)
target_link_libraries(exception_test pthread)
