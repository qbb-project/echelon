# - Try to find echelon
#
#  Once done this will define
#  Echelon_ROOT - Hint for the find module
#  ECHELON_FOUND - System has Echelon
#  Echelon_INCLUDE_DIRS - The Echelon include directories
#  Echelon_LIBRARIES - The libraries needed to use Echelon
#  Echelon_DEFINITIONS - Compiler switches required for using Echelon

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

set(Echelon_DEFINITIONS )

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
