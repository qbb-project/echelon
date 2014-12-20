set(COVERAGE_ENABLE OFF CACHE BOOL "Enable test coverage support.")

if(COVERAGE_ENABLE)
  set(COVERAGE_FLAGS $<$<CXX_COMPILER_ID:GNU>:-fprofile-arcs -ftest-coverage>)
  set(COVERAGE_LINKER_FLAGS $<$<CXX_COMPILER_ID:GNU>:-fprofile-arcs -ftest-coverage>)
endif()
