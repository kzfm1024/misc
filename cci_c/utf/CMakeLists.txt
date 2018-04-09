cmake_minimum_required(VERSION 2.8)
project(cci_c)

#set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11")

add_executable(cci_c cci.c cci_pars.c cci_tkn.c cci_tbl.c cci_code.c cci_misc.c)
#target_link_libraries(blocking_queue pthread)
