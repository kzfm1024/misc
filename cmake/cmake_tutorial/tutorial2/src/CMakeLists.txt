cmake_minimum_required (VERSION 2.8)
project (tutorial2)

option (USE_MYMATH "Use tutorial provided math implementation" ON) 
if (USE_MYMATH)
  add_definitions (-DUSE_MYMATH)
  include_directories (${PROJECT_SOURCE_DIR}/../../mymath/include)
  add_subdirectory (${PROJECT_SOURCE_DIR}/../../mymath/src mymath)
  set (EXTRA_LIBS ${EXTRA_LIBS} mymath m)
endif (USE_MYMATH)

add_executable (tutorial2 tutorial2.cpp)
target_link_libraries (tutorial2 ${EXTRA_LIBS})
