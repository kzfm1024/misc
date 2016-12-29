cmake_minimum_required (VERSION 2.8)
project (mymath)

include_directories (${PROJECT_SOURCE_DIR}/../include)
add_library (mymath STATIC mysqrt.c)
