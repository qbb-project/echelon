#  Copyright (c) 2012-2014 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(ECHELON_BUILD_TESTS OFF CACHE Bool "build the test suite")

if(ECHELON_BUILD_TESTS)
  find_package(Boost 1.50.0 COMPONENTS unit_test_framework REQUIRED)

  add_executable(creation_and_access_test creation_and_access.cpp)
  target_include_directories(creation_and_access_test PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(creation_and_access_test echelon ${Boost_LIBRARIES})
  add_test(creation_and_access_test ${CMAKE_CURRENT_BINARY_DIR}/creation_and_access_test)

  add_executable(basic_io_test basic_io.cpp)
  target_include_directories(basic_io_test PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(basic_io_test echelon ${Boost_LIBRARIES})
  add_test(basic_io_test ${CMAKE_CURRENT_BINARY_DIR}/basic_io_test)

  add_executable(string_support_test string_support.cpp)
  target_include_directories(string_support_test PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(string_support_test echelon ${Boost_LIBRARIES})
  add_test(string_support_test ${CMAKE_CURRENT_BINARY_DIR}/string_support_test)

  add_executable(object_reference_handling object_reference_handling.cpp)
  target_include_directories(object_reference_handling PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(object_reference_handling echelon ${Boost_LIBRARIES})
  add_test(object_reference_handling ${CMAKE_CURRENT_BINARY_DIR}/error_handling)

  add_executable(error_handling error_handling.cpp)
  target_include_directories(error_handling PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(error_handling echelon ${Boost_LIBRARIES})
  add_test(error_handling ${CMAKE_CURRENT_BINARY_DIR}/error_handling)

  add_executable(slicing slicing.cpp)
  target_include_directories(slicing PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(slicing echelon ${Boost_LIBRARIES})
  add_test(slicing ${CMAKE_CURRENT_BINARY_DIR}/slicing)

  add_executable(type_equality type_equality.cpp)
  target_include_directories(type_equality PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(type_equality echelon ${Boost_LIBRARIES})
  add_test(type_equality ${CMAKE_CURRENT_BINARY_DIR}/type_equality)

  add_executable(require_api require_api.cpp)
  target_include_directories(require_api PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(require_api echelon ${Boost_LIBRARIES})
  add_test(require_api ${CMAKE_CURRENT_BINARY_DIR}/require_api)

  add_executable(sink_adaptors sink_adaptors.cpp)
  target_include_directories(sink_adaptors PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(sink_adaptors echelon ${Boost_LIBRARIES})
  add_test(sink_adaptors ${CMAKE_CURRENT_BINARY_DIR}/sink_adaptors)

  add_executable(null_state null_state.cpp)
  target_include_directories(null_state PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(null_state echelon ${Boost_LIBRARIES})
  add_test(null_state ${CMAKE_CURRENT_BINARY_DIR}/null_state)
  
  add_executable(dynamic_shape dynamic_shape.cpp)
  target_include_directories(dynamic_shape PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries(dynamic_shape echelon ${Boost_LIBRARIES})
  add_test(dynamic_shape ${CMAKE_CURRENT_BINARY_DIR}/dynamic_shape)
endif()