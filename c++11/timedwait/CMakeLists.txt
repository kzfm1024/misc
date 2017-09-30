cmake_minimum_required(VERSION 2.8)
project(timedwait)

#set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

add_executable(timedwait timedwait.c)
target_link_libraries(timedwait pthread)
