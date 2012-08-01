
file(GLOB echelon_hdf5_header_files ../../echelon/hdf5/*.hpp)
file(GLOB echelon_hdf5_source_files ./*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_library(echelon_hdf5 ${echelon_hdf5_header_files} ${echelon_hdf5_source_files})