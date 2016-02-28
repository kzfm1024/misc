cmake_minimum_required(VERSION 2.8)
project(mps)

#set(CMAKE_C_FLAGS "-g -O2")
set(CMAKE_C_FLAGS "-g")

#add_executable(mps mps.c)
add_executable(mps mps-commentary.c)
target_link_libraries(mps m)
