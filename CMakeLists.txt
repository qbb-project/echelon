cmake_minimum_required (VERSION 2.6)
enable_testing()

project (echelon)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif() 

set(CMAKE_MODULE_PATH ${echelon_SOURCE_DIR}/cmake)

include(generate_imported_library_targets)

find_package(Boost COMPONENTS serialization unit_test_framework REQUIRED)
find_package(HDF5 COMPONENTS C HL REQUIRED)

################################################################################################################################
## create imported targets for all non-system libraries                                                                       ##
##                                                                                                                            ##
## Rational: CMake treats all libraries in implicit system locations as system libraries,                                     ##
##           even if they are 'normal' libraries. (see http://www.cmake.org/Wiki/CMake_2.6_Notes#Linking_to_System_Libraries  ##
##           for a more detailed explanation)                                                                                 ##
##                                                                                                                            ##
##           Due to this the linker might link against the wrong libraries, if several versions of a library are installed,   ##
##           which in turn might be out-dated or incompatible with the header files found by find_package.                    ##
##           The imported target forces the linker to use the libraries found by find_package.                                ##
##                                                                                                                            ##
################################################################################################################################

generate_imported_library_targets(Boost_LIBRARIES boost_targets)
generate_imported_library_targets(HDF5_LIBRARIES hdf5_targets)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
