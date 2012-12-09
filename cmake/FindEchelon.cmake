#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# - Try to find echelon
#
#  Once done this will define
#  Echelon_ROOT - Hint for the find module
#  ECHELON_FOUND - System has Echelon
#  Echelon_INCLUDE_DIRS - The Echelon include directories
#  Echelon_LIBRARIES - The libraries needed to use Echelon
#  Echelon_DEFINITIONS - Compiler switches required for using Echelon

include(TestCXXAcceptsFlag)

if(Echelon_FIND_QUIETLY AND NOT Echelon_FIND_REQUIRED)
  find_package(HDF5 COMPONENTS C HL QUIET)
elseif(NOT Echelon_FIND_QUIETLY AND Echelon_FIND_REQUIRED)
  find_package(HDF5 COMPONENTS C HL REQUIRED)
elseif(Echelon_FIND_QUIETLY AND Echelon_FIND_REQUIRED)
  find_package(HDF5 COMPONENTS C HL QUIET REQUIRED)
else()
  find_package(HDF5 COMPONENTS C HL)
endif()

if(Echelon_FIND_QUIETLY AND NOT Echelon_FIND_REQUIRED)
  find_package(Boost COMPONENTS serialization unit_test_framework QUIET)
elseif(NOT Echelon_FIND_QUIETLY AND Echelon_FIND_REQUIRED)
  find_package(Boost COMPONENTS serialization unit_test_framework REQUIRED)
elseif(Echelon_FIND_QUIETLY AND Echelon_FIND_REQUIRED)
  find_package(Boost COMPONENTS serialization unit_test_framework QUIET REQUIRED)
else()
  find_package(Boost COMPONENTS serialization unit_test_framework)
endif()

CHECK_CXX_ACCEPTS_FLAG("-std=c++11" COMPILER_ACCEPTS_GCC_CPP11_FLAG)

if(COMPILER_ACCEPTS_GCC_CPP11_FLAG)
    set(Echelon_DEFINITIONS "-std=c++11")
else(COMPILER_ACCEPTS_GCC_CPP11_FLAG)

    CHECK_CXX_ACCEPTS_FLAG("-std=c++0x" COMPILER_ACCEPTS_GCC_CPP0X_FLAG)

    if(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
        set(Echelon_DEFINITIONS "-std=c++0x")
    else(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
        set(Echelon_DEFINITIONS )
    endif(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)

endif(COMPILER_ACCEPTS_GCC_CPP11_FLAG)

find_path(Echelon_INCLUDE_DIR echelon/file.hpp
          HINTS ENV Echelon_ROOT
          HINTS ${Echelon_ROOT}
          PATH_SUFFIXES include)

find_library(Echelon_LIBRARY NAMES echelon echelon_hdf5
             HINTS ENV Echelon_ROOT
             HINTS ${Echelon_ROOT}
             PATH_SUFFIXES lib)

set(Echelon_LIBRARIES ${Echelon_LIBRARY} ${HDF5_LIBRARIES} ${Boost_LIBRARIES})
set(Echelon_INCLUDE_DIRS ${Echelon_INCLUDE_DIR} ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set Echelon_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Echelon DEFAULT_MSG
                                  Echelon_LIBRARY Echelon_INCLUDE_DIR)

mark_as_advanced(Echelon_INCLUDE_DIR Echelon_LIBRARY) 
