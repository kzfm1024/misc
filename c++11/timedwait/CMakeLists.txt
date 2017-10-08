cmake_minimum_required(VERSION 2.8)
project(timedwait)

set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

add_executable(sleep_test sleep_test.cpp)
target_link_libraries(sleep_test pthread)

add_executable(timedwait timedwait.c)
target_link_libraries(timedwait pthread)

add_executable(std_wait_until std_wait_until.cpp)
target_link_libraries(std_wait_until pthread)


#find_package(Boost)
#if(Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIRS})
#  add_executable(boost_wait_until boost_wait_until.cpp)
#  target_link_libraries(boost_wait_until boost_thread boost_chrono boost_system pthread)
#endif()

add_executable(boost_wait_until boost_wait_until.cpp)
target_link_libraries(boost_wait_until boost_thread boost_chrono boost_system pthread)
