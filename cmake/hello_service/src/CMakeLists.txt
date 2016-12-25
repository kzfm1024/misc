cmake_minimum_required(VERSION 2.8)
project(hello_service)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DMYLOG_DEBUG)
endif()

add_executable(hello_service hello_service.c)

# mymath library
include_directories(${PROJECT_SOURCE_DIR}/../../mymath/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../mymath/src mymath)
target_link_libraries(hello_service mymath m)

install(
  PROGRAMS ${CMAKE_BINARY_DIR}/hello_service
  DESTINATION ${CMAKE_SOURCE_DIR}/../systemd
  )
