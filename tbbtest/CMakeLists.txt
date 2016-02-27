cmake_minimum_required(VERSION 2.8)
project(tbbtest)

if(USE_XEON_PHI)
set(CMAKE_CXX_FLAGS "-mmic -O2 -std=c++11")
else()
set(CMAKE_CXX_FLAGS "-O2 -fpermissive -std=c++11")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")

add_executable(tbbtest tbbtest.cpp)
target_link_libraries(tbbtest tbb pthread m)
