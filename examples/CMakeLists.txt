set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(../ ${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})

add_executable(example1 example1.cpp) 

target_link_libraries(example1 echelon) 
