#  Copyright (c) 2012-2019 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(ECHELON_BUILD_TESTS OFF CACHE BOOL "build the test suite")

function(echelon_add_test name src_file)
    add_executable(${name} ${src_file})
    target_include_directories(${name} PUBLIC ${Boost_INCLUDE_DIRS})
    target_compile_options(${name} PRIVATE ${COVERAGE_FLAGS})
    target_link_libraries(${name} PUBLIC echelon ${Boost_LIBRARIES} ${COVERAGE_LINKER_FLAGS})
    add_test(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name})
endfunction()

function(echelon_add_simple_test name)
    echelon_add_test(${name} ${name}.cpp)
endfunction()

if(ECHELON_BUILD_TESTS)
    find_package(Boost 1.50.0 COMPONENTS unit_test_framework REQUIRED)

    echelon_add_simple_test(creation_and_access)
    echelon_add_simple_test(basic_io)
    echelon_add_simple_test(string_support)
    echelon_add_simple_test(object_reference_handling)
    echelon_add_simple_test(error_handling)
    echelon_add_simple_test(slicing)
    echelon_add_simple_test(type_equality)
    echelon_add_simple_test(require_api)
    echelon_add_simple_test(sink_adaptors)
    echelon_add_simple_test(null_state)
    echelon_add_simple_test(dynamic_shape)
    echelon_add_simple_test(array_slicing)
    echelon_add_simple_test(layout_handling)

    add_subdirectory(regression)
endif()
