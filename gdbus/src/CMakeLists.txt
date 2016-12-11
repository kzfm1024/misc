cmake_minimum_required(VERSION 2.8)
project(gdbus-example)

find_package(PkgConfig)
pkg_check_modules(GIO REQUIRED gio-unix-2.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

include_directories(${GIO_INCLUDE_DIRS})
link_directories(${GIO_LIBRARY_DIRS})
add_definitions(${GIO_CFLAGS_OTHER})

add_executable(objectmanager-server gdbus-example-objectmanager-server.c generated-code.c)
target_link_libraries(objectmanager-server ${GIO_LIBRARIES})

add_executable(objectmanager-client gdbus-example-objectmanager-client.c generated-code.c)
target_link_libraries(objectmanager-client ${GIO_LIBRARIES})
