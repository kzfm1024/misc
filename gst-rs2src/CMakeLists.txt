cmake_minimum_required(VERSION 3.1.0)
project(gst-rs2src)

find_package(PkgConfig)
pkg_check_modules(GST REQUIRED
  gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
#set_property(TARGET gstrs2src PROPERTY CXX_STANDARD 11)

include_directories(${GST_INCLUDE_DIRS})

add_library(gstrs2src SHARED gstrs2src.c)
target_link_libraries(gstrs2src realsense2 ${GST_LIBRARIES})

add_library(gstnumbersrc SHARED gstnumbersrc.c)
target_link_libraries(gstnumbersrc ${GST_LIBRARIES})

add_library(gstnumbersink SHARED gstnumbersink.c)
target_link_libraries(gstnumbersink ${GST_LIBRARIES})
