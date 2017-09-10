cmake_minimum_required(VERSION 2.8)
project(c++11)

set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

add_executable(blocking_queue blocking_queue.cpp)
#add_executable(weak_ptr weak_ptr.cpp)
#add_executable(hoge_manager hoge_manager.cpp)

