#  Copyright (c) 2012-2016 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

find_package(HDF5 COMPONENTS C HL REQUIRED)

file(GLOB echelon_hdf5_header_files ../../echelon/hdf5/*.hpp ../../echelon/hdf5/precursor/*.hpp)
file(GLOB echelon_hdf5_source_files ./*.cpp ./precursor/*.cpp)

if(NOT ${ECHELON_BUILD_STATIC})
    add_library(echelon_hdf5 SHARED ${echelon_hdf5_header_files} ${echelon_hdf5_source_files})
else()
    add_library(echelon_hdf5 STATIC ${echelon_hdf5_header_files} ${echelon_hdf5_source_files})
endif()

set_property(TARGET echelon_hdf5 PROPERTY VERSION 0.8.0)
set_property(TARGET echelon_hdf5 PROPERTY SOVERSION 0)
set_property(TARGET echelon_hdf5 PROPERTY INTERFACE_echelon_hdf5_MAJOR_VERSION 0)
set_property(TARGET echelon_hdf5 APPEND PROPERTY COMPATIBLE_INTERFACE_STRING echelon_hdf5_MAJOR_VERSION)
set_property(TARGET echelon_hdf5 PROPERTY INTERFACE_echelon_hdf5_MINOR_VERSION 8)
set_property(TARGET echelon_hdf5 APPEND PROPERTY COMPATIBLE_INTERFACE_STRING echelon_hdf5_MINOR_VERSION)

target_include_directories(echelon_hdf5 PUBLIC ${Boost_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS} $<BUILD_INTERFACE:${ECHELON_ROOT_DIR}>)
target_link_libraries(echelon_hdf5 PUBLIC ${HDF5_LIBRARIES} ${COVERAGE_LINKER_FLAGS})
target_compile_options(echelon_hdf5 PRIVATE -pedantic -Wall -Wextra ${COVERAGE_FLAGS})
# Activate the compatibility mode until we have ported the code to the new interface
# introduced in HDF5 1.10.
set(echelon_hdf5_compile_definitions "-DH5_USE_18_API")

if (ECHELON_ENABLE_1_0_FORMAT_COMPATIBILITY)
    set(echelon_hdf5_compile_definitions "${echelon_hdf5_compile_definitions} -DECHELON_HAVE_1_0_FORMAT_COMPATIBILITY")
endif()

target_compile_definitions(echelon_hdf5 PUBLIC ${echelon_hdf5_compile_definitions})

if(CMAKE_VERSION VERSION_LESS "3.1.0")
    target_compile_options(echelon_hdf5 PUBLIC -std=c++11)
else()
    target_compile_features(echelon_hdf5 PUBLIC cxx_variadic_templates cxx_auto_type cxx_decltype
                                                cxx_defaulted_functions cxx_deleted_functions
                                                cxx_explicit_conversions cxx_lambdas cxx_noexcept
                                                cxx_nullptr cxx_override cxx_final cxx_range_for
                                                cxx_right_angle_brackets cxx_rvalue_references cxx_static_assert
                                                cxx_strong_enums cxx_trailing_return_types)
endif()

if(NOT ECHELON_IS_A_SUBPROJECT)
    install(TARGETS echelon_hdf5 EXPORT echelon-targets
                                 RUNTIME DESTINATION bin
                                 INCLUDES DESTINATION include
                                 LIBRARY DESTINATION lib
                                 ARCHIVE DESTINATION lib)
else()
    install(TARGETS echelon_hdf5 EXPORT echelon-targets OPTIONAL
                                 RUNTIME DESTINATION bin
                                 INCLUDES DESTINATION include
                                 LIBRARY DESTINATION lib
                                 ARCHIVE DESTINATION lib)
                                 
    add_library(echelon::echelon_hdf5 ALIAS echelon_hdf5)
endif()
