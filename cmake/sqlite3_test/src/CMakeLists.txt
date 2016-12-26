cmake_minimum_required (VERSION 2.8)
project (sqlite3_test)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

find_package(PkgConfig)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

include_directories(${SQLITE3_INCLUDES})
add_executable(sqlite3_test sqlite3_test.c)
target_link_libraries(sqlite3_test ${SQLITE3_LIBRARIES})
