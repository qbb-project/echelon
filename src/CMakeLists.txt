include_directories(../)

add_subdirectory(hdf5)

file(GLOB echelon_header_files ../echelon/*.hpp ../echelon/std/*.hpp ../echelon/detail/*.hpp)
file(GLOB echelon_source_files ./*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wall -Wextra")

include_directories(${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})

add_library(echelon ${echelon_header_files} ${echelon_source_files}) 
target_link_libraries(echelon echelon_hdf5 ${boost_targets})

install(DIRECTORY ../echelon/ DESTINATION include/echelon
        FILES_MATCHING PATTERN "*.hpp")
install(TARGETS echelon DESTINATION lib)

# mimic the behaviour of the find module, if echelon is used as an internal dependency
set(Echelon_DEFINITIONS )
set(ECHELON_FOUND 1)
set(Echelon_INCLUDE_DIR ${echelon_SOURCE_DIR})
set(Echelon_LIBRARY echelon)

set(Echelon_LIBRARIES ${Echelon_LIBRARY})
set(Echelon_INCLUDE_DIRS ${Echelon_INCLUDE_DIR} ${HDF5_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

mark_as_advanced(Echelon_INCLUDE_DIR Echelon_LIBRARY)