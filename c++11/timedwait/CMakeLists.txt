cmake_minimum_required(VERSION 2.8)
project(timedwait)

set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

add_executable(sleep_test sleep_test.cpp)
target_link_libraries(sleep_test pthread)

add_executable(select_test select_test.cpp)
target_link_libraries(select_test pthread)

add_executable(poll_test poll_test.cpp)
target_link_libraries(poll_test pthread)

add_executable(epoll_test epoll_test.cpp)
target_link_libraries(epoll_test pthread)

add_executable(pthread_cond_test pthread_cond_test.c)
target_link_libraries(pthread_cond_test pthread)

add_executable(std_condition_variable_test std_condition_variable_test.cpp)
target_link_libraries(std_condition_variable_test pthread)

add_executable(boost_condition_variable_test boost_condition_variable_test.cpp)
target_link_libraries(boost_condition_variable_test boost_thread boost_chrono boost_system pthread)

add_executable(boost_condition_variable_steady_test boost_condition_variable_steady_test.cpp)
target_link_libraries(boost_condition_variable_steady_test boost_thread boost_chrono boost_system pthread)
