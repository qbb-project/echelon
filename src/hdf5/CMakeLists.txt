#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

file(GLOB echelon_hdf5_header_files ../../echelon/hdf5/*.hpp ../../echelon/hdf5/precursor/*.hpp)
file(GLOB echelon_hdf5_source_files ./*.cpp ./precursor/*.cpp)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_library(echelon_hdf5 ${echelon_hdf5_header_files} ${echelon_hdf5_source_files})
target_include_directories(echelon_hdf5 PUBLIC ${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS} PRIVATE ${ECHELON_ROOT_DIR})
target_link_libraries(echelon_hdf5 ${hdf5_targets})

set_target_properties(echelon_hdf5 PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

if(NOT ECHELON_IS_A_SUBPROJECT)
    install(TARGETS echelon_hdf5 DESTINATION lib)
else()
    install(TARGETS echelon_hdf5 LIBRARY DESTINATION lib
                                 ARCHIVE DESTINATION lib)
endif()