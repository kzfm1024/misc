cmake_minimum_required(VERSION 2.8)
project(sound)

set(CMAKE_CXX_FLAGS "-g -O2 -D_REENTRANT")

add_executable(pulsedevicelist pulsedevicelist.c)
target_link_libraries(pulsedevicelist pulse m)

add_executable(pa-beep pa-beep.c)
target_link_libraries(pa-beep pulse m)

add_executable(pacat-simple pacat-simple.c)
target_link_libraries(pacat-simple pulse-simple pulse m)

add_executable(parec-simple parec-simple.c)
target_link_libraries(parec-simple pulse-simple pulse m)
