#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

include(TestCXXAcceptsFlag)

add_subdirectory(hdf5)

file(GLOB echelon_header_files ../echelon/*.hpp ../echelon/std/*.hpp ../echelon/detail/*.hpp ../echelon/utility/*.hpp)
file(GLOB echelon_source_files ./*.cpp)

add_library(echelon ${echelon_header_files} ${echelon_source_files})

target_include_directories(echelon PUBLIC ${Boost_INCLUDE_DIRS} echelon_hdf5 ${ECHELON_ROOT_DIR})
target_link_libraries(echelon  echelon_hdf5 ${boost_targets})

set_target_properties(echelon PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

# CHECK_CXX_ACCEPTS_FLAG("-std=c++11" COMPILER_ACCEPTS_GCC_CPP11_FLAG)
# 
# if(COMPILER_ACCEPTS_GCC_CPP11_FLAG)
#     set(Echelon_DEFINITIONS "-std=c++11" PARENT_SCOPE)
# else(COMPILER_ACCEPTS_GCC_CPP11_FLAG)
# 
#     CHECK_CXX_ACCEPTS_FLAG("-std=c++0x" COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
# 
#     if(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
#         set(Echelon_DEFINITIONS "-std=c++0x" PARENT_SCOPE)
#     else(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
#         
#     endif(COMPILER_ACCEPTS_GCC_CPP0X_FLAG)
# 
# endif(COMPILER_ACCEPTS_GCC_CPP11_FLAG)

install(DIRECTORY ../echelon/ DESTINATION include/echelon
        FILES_MATCHING PATTERN "*.hpp")
install(TARGETS echelon DESTINATION lib)
