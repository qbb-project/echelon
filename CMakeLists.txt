cmake_minimum_required (VERSION 2.6)

project (echelon)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif() 

add_subdirectory(src)
add_subdirectory(examples)