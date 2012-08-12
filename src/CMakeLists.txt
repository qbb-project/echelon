include_directories(../)

add_subdirectory(hdf5)

file(GLOB echelon_header_files ../echelon/*.hpp)
file(GLOB echelon_source_files ./*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})

add_library(echelon ${echelon_header_files} ${echelon_source_files}) 
target_link_libraries(echelon echelon_hdf5 ${Boost_LIBRARIES})