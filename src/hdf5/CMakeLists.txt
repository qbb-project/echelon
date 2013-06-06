#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

file(GLOB echelon_hdf5_header_files ../../echelon/hdf5/*.hpp)
file(GLOB echelon_hdf5_source_files ./*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})

add_library(echelon_hdf5 ${echelon_hdf5_header_files} ${echelon_hdf5_source_files})
target_link_libraries(echelon_hdf5 ${hdf5_targets})

if(NOT ECHELON_IS_A_SUBPROJECT)
    install(TARGETS echelon_hdf5 DESTINATION lib)
else()
    install(TARGETS echelon_hdf5 LIBRARY DESTINATION lib
                                 ARCHIVE DESTINATION lib)
endif()