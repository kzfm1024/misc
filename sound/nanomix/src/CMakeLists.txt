cmake_minimum_required (VERSION 2.8)
project (nanomix)

#include_directories (${PROJECT_SOURCE_DIR}/../include)
#add_library (nanomixer STATIC nanomixer.c)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

add_executable (nanomix nanomix.c)
target_link_libraries (nanomix asound)

