set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(../ ${HDF5_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})

add_executable(creation_and_access_test creation_and_access.cpp) 
target_link_libraries(creation_and_access_test echelon ${BOOST_LIBRARIES})

add_test(creation_and_access_test ${CMAKE_CURRENT_BINARY_DIR}/creation_and_access_test)


add_executable(basic_io_test basic_io.cpp) 
target_link_libraries(basic_io_test echelon ${BOOST_LIBRARIES})

add_test(basic_io_test ${CMAKE_CURRENT_BINARY_DIR}/basic_io_test)


add_executable(string_support_test string_support.cpp) 
target_link_libraries(string_support_test echelon ${BOOST_LIBRARIES})

add_test(string_support_test ${CMAKE_CURRENT_BINARY_DIR}/string_support_test)


add_executable(error_handling error_handling.cpp) 
target_link_libraries(error_handling echelon ${BOOST_LIBRARIES})

add_test(error_handling ${CMAKE_CURRENT_BINARY_DIR}/error_handling)
