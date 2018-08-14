cmake_minimum_required(VERSION 2.8)
project(atomic_flag)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(atomic_flag atomic_flag.cpp)
target_link_libraries(atomic_flag pthread)

add_library(lfjournal lf-journal.cpp)

add_executable(lf-journald lf-journald.cpp)
target_link_libraries(lf-journald lfjournal)

add_executable(test-lfjournal test-lfjournal.cpp)
target_link_libraries(test-lfjournal lfjournal)

add_executable(print-lfjournal print-lfjournal.cpp)
target_link_libraries(print-lfjournal lfjournal)
