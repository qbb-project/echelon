#  Copyright (c) 2015 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

if(ECHELON_BUILD_TESTS)

    add_executable(static_shape_writeable static_shape_writeable.cpp)
    target_include_directories(static_shape_writeable PUBLIC ${Boost_INCLUDE_DIRS})
    target_compile_options(static_shape_writeable PRIVATE ${COVERAGE_FLAGS})
    target_link_libraries(static_shape_writeable echelon ${Boost_LIBRARIES} ${COVERAGE_LINKER_FLAGS})
    add_test(static_shape_writeable ${CMAKE_CURRENT_BINARY_DIR}/static_shape_writeable)

endif()