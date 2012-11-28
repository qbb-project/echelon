set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(../ ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_executable(creation_and_access_test creation_and_access.cpp) 
target_link_libraries(creation_and_access_test echelon)

add_test(creation_and_access_test ${CMAKE_CURRENT_BINARY_DIR}/creation_and_access_test)


add_executable(basic_io_test basic_io.cpp) 
target_link_libraries(basic_io_test echelon)

add_test(basic_io_test ${CMAKE_CURRENT_BINARY_DIR}/basic_io_test)


add_executable(string_support_test string_support.cpp) 
target_link_libraries(string_support_test echelon)

add_test(string_support_test ${CMAKE_CURRENT_BINARY_DIR}/string_support_test)


add_executable(error_handling error_handling.cpp) 
target_link_libraries(error_handling echelon)

add_test(error_handling ${CMAKE_CURRENT_BINARY_DIR}/error_handling)


add_executable(slicing slicing.cpp) 
target_link_libraries(slicing echelon)

add_test(slicing ${CMAKE_CURRENT_BINARY_DIR}/slicing)


add_executable(type_equality type_equality.cpp) 
target_link_libraries(type_equality echelon)

add_test(type_equality ${CMAKE_CURRENT_BINARY_DIR}/type_equality)


add_executable(require_api require_api.cpp) 
target_link_libraries(require_api echelon)

add_test(require_api ${CMAKE_CURRENT_BINARY_DIR}/require_api)