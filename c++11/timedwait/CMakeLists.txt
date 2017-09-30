cmake_minimum_required(VERSION 2.8)
project(timedwait)

set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

add_executable(timedwait timedwait.c)
add_executable(wait_until wait_until.cpp)
target_link_libraries(timedwait pthread)
target_link_libraries(wait_until pthread)
