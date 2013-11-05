#  Copyright (c) 2012-2013 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

add_executable(creation_and_access_test EXCLUDE_FROM_ALL creation_and_access.cpp) 
target_link_libraries(creation_and_access_test echelon)

add_test(creation_and_access_test ${CMAKE_CURRENT_BINARY_DIR}/creation_and_access_test)
add_dependencies(check creation_and_access_test)

add_executable(basic_io_test EXCLUDE_FROM_ALL basic_io.cpp)
target_link_libraries(basic_io_test echelon)

add_test(basic_io_test ${CMAKE_CURRENT_BINARY_DIR}/basic_io_test)
add_dependencies(check basic_io_test)

add_executable(string_support_test EXCLUDE_FROM_ALL string_support.cpp)
target_link_libraries(string_support_test echelon)

add_test(string_support_test ${CMAKE_CURRENT_BINARY_DIR}/string_support_test)
add_dependencies(check string_support_test)

add_executable(error_handling EXCLUDE_FROM_ALL error_handling.cpp)
target_link_libraries(error_handling echelon)

add_test(error_handling ${CMAKE_CURRENT_BINARY_DIR}/error_handling)
add_dependencies(check error_handling)

add_executable(slicing EXCLUDE_FROM_ALL slicing.cpp)
target_link_libraries(slicing echelon)

add_test(slicing ${CMAKE_CURRENT_BINARY_DIR}/slicing)
add_dependencies(check slicing)

add_executable(type_equality EXCLUDE_FROM_ALL type_equality.cpp)
target_link_libraries(type_equality echelon)

add_test(type_equality ${CMAKE_CURRENT_BINARY_DIR}/type_equality)
add_dependencies(check type_equality)

add_executable(require_api EXCLUDE_FROM_ALL require_api.cpp)
target_link_libraries(require_api echelon)

add_test(require_api ${CMAKE_CURRENT_BINARY_DIR}/require_api)
add_dependencies(check require_api)