set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

find_package(HDF5 COMPONENTS C HL REQUIRED)

include_directories(../ ${HDF5_INCLUDE_DIRS})

add_executable(example1 example1.cpp) 

target_link_libraries(example1 echelon ${HDF5_LIBRARIES}) 
