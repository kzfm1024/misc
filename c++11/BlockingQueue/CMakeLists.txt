cmake_minimum_required(VERSION 2.8)
project(blocking_queue)

set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

add_executable(blocking_queue blocking_queue.cpp)
target_link_libraries(blocking_queue pthread)
