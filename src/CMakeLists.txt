#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

include(TestCXXAcceptsFlag)

include_directories(../)

add_subdirectory(hdf5)

file(GLOB echelon_header_files ../echelon/*.hpp ../echelon/std/*.hpp ../echelon/detail/*.hpp ../echelon/utility/*.hpp)
file(GLOB echelon_source_files ./*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wall -Wextra")

include_directories(${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})

add_library(echelon ${echelon_header_files} ${echelon_source_files}) 
target_link_libraries(echelon echelon_hdf5 ${boost_targets})

if(NOT ECHELON_IS_A_SUBPROJECT)

    install(DIRECTORY ../echelon/ DESTINATION include/echelon
            FILES_MATCHING PATTERN "*.hpp")
    install(TARGETS echelon DESTINATION lib)

else()

    # mimic the behaviour of the find module, if echelon is used as an internal dependency
    set(ECHELON_FOUND 1 PARENT_SCOPE)
    set(Echelon_INCLUDE_DIR ${echelon_SOURCE_DIR} CACHE PATH "echelon's include directories")
    set(Echelon_LIBRARY echelon CACHE STRING "echelon's libraries")

    CHECK_CXX_ACCEPTS_FLAG("-std=c++11" COMPILER_ACCEPTS_GCC_CPP11_FLAG)

    if(COMPILER_ACCEPTS_GCC_CPP11_FLAG)
        set(Echelon_DEFINITIONS "-std=c++11" PARENT_SCOPE)
    else(COMPILER_ACCEPTS_GCC_CPP11_FLAG)

        CHECK_CXX_ACCEPTS_FLAG("-std=c++0x" COMPILER_ACCEPTS_GCC_CPP0X_FLAG)

        if(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
            set(Echelon_DEFINITIONS "-std=c++0x" PARENT_SCOPE)
        else(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
            set(Echelon_DEFINITIONS PARENT_SCOPE)
        endif(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)

    endif(COMPILER_ACCEPTS_GCC_CPP11_FLAG)

    set(Echelon_LIBRARIES ${Echelon_LIBRARY} PARENT_SCOPE)
    set(Echelon_INCLUDE_DIRS ${Echelon_INCLUDE_DIR} ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} PARENT_SCOPE)

    mark_as_advanced(Echelon_INCLUDE_DIR Echelon_LIBRARY)

endif()