cmake_minimum_required (VERSION 2.8)
project (nanomix)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

add_executable (nanomix nanomix.c)
target_link_libraries (nanomix asound)
