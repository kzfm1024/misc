cmake_minimum_required(VERSION 2.8)
project(mps)

set(CMAKE_CC_FLAGS "-g -O2")
#set(CMAKE_CC_FLAGS_DEBUG "-g")

add_executable(mps mps.c)
target_link_libraries(mps m)
