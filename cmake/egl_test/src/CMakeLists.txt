cmake_minimum_required (VERSION 2.8)
project (egl_test)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

add_executable(egl_test egl_test.cpp)
target_link_libraries(egl_test EGL GL)
