#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required (VERSION 2.6)
enable_testing()

project (echelon)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif() 

set(CMAKE_MODULE_PATH ${echelon_SOURCE_DIR}/cmake)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

include(generate_imported_library_targets)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(ECHELON_IS_A_SUBPROJECT 0)
else()
  set(ECHELON_IS_A_SUBPROJECT 1)
endif()

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

if(NOT ECHELON_IS_A_SUBPROJECT)

    add_subdirectory(examples)
    add_subdirectory(tests)

endif()

if(ECHELON_IS_A_SUBPROJECT)

    set(Echelon_DEFINITIONS PARENT_SCOPE)
    set(ECHELON_FOUND ${ECHELON_FOUND} PARENT_SCOPE)
    set(Echelon_LIBRARIES ${Echelon_LIBRARIES} PARENT_SCOPE)
    set(Echelon_INCLUDE_DIRS ${Echelon_INCLUDE_DIRS} PARENT_SCOPE)

endif()

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)

  if(NOT ECHELON_IS_A_SUBPROJECT)
    add_custom_target(doc
                      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
                      COMMENT "Generating API documentation with Doxygen." VERBATIM)
  else(NOT ECHELON_IS_A_SUBPROJECT)
    add_custom_target(echelon_doc
                      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
                      COMMENT "Generating API documentation with Doxygen." VERBATIM)
  endif(NOT ECHELON_IS_A_SUBPROJECT)

endif(DOXYGEN_FOUND)