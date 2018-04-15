cmake_minimum_required(VERSION 3.5)
project(cci_c)

add_executable(cci_c cci.c cci_pars.c cci_tkn.c cci_tbl.c cci_code.c cci_misc.c)
add_executable(minicalc minicalc.c)
add_executable(polish_p polish_p.c)
add_executable(token_p token_p.c)
