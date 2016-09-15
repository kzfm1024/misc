cmake_minimum_required(VERSION 2.8)
project(sound)

set(CMAKE_CXX_FLAGS "-g -O2 -D_REENTRANT")

add_executable(pulsedevicelist pulsedevicelist.c)
target_link_libraries(pulsedevicelist pulse m)

add_executable(pa-beep pa-beep.c)
target_link_libraries(pa-beep pulse m)
