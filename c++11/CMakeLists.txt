cmake_minimum_required(VERSION 2.8)
project(c++11)

#set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gpu_physics)

#add_definitions("-DCSV_DUMP")
add_executable(shared_ptr shared_ptr.cpp)
add_executable(weak_ptr weak_ptr.cpp)
add_executable(hoge_manager hoge_manager.cpp)
