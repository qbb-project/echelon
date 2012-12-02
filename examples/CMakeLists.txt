#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(../ ${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})

add_executable(example1 example1.cpp) 

target_link_libraries(example1 echelon) 
