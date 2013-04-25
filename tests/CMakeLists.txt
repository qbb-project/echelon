#  Copyright (c) 2012 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

add_executable(creation_and_access_test creation_and_access.cpp) 
target_link_libraries(creation_and_access_test echelon)
set_target_properties(creation_and_access_test PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

add_test(creation_and_access_test ${CMAKE_CURRENT_BINARY_DIR}/creation_and_access_test)


add_executable(basic_io_test basic_io.cpp)
target_link_libraries(basic_io_test echelon)
set_target_properties(basic_io_test PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

add_test(basic_io_test ${CMAKE_CURRENT_BINARY_DIR}/basic_io_test)


add_executable(string_support_test string_support.cpp)
target_link_libraries(string_support_test echelon)
set_target_properties(string_support_test PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

add_test(string_support_test ${CMAKE_CURRENT_BINARY_DIR}/string_support_test)


add_executable(error_handling error_handling.cpp)
target_link_libraries(error_handling echelon)
set_target_properties(error_handling PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

add_test(error_handling ${CMAKE_CURRENT_BINARY_DIR}/error_handling)


add_executable(slicing slicing.cpp)
target_link_libraries(slicing echelon)
set_target_properties(slicing PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

add_test(slicing ${CMAKE_CURRENT_BINARY_DIR}/slicing)


add_executable(type_equality type_equality.cpp)
target_link_libraries(type_equality echelon)
set_target_properties(type_equality PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

add_test(type_equality ${CMAKE_CURRENT_BINARY_DIR}/type_equality)


add_executable(require_api require_api.cpp)
target_link_libraries(require_api echelon)
set_target_properties(require_api PROPERTIES COMPILE_FLAGS "-std=c++11 -pedantic -Wall -Wextra")

add_test(require_api ${CMAKE_CURRENT_BINARY_DIR}/require_api)