#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_executable(example1 example1.cpp) 
target_link_libraries(example1 echelon)
set_target_properties(example1 PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")
