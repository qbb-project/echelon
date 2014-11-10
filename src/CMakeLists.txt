#  Copyright (c) 2012-2014 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

include(TestCXXAcceptsFlag)
include(GenerateExportHeader)

find_package(Boost 1.50.0 COMPONENTS serialization REQUIRED)

add_subdirectory(hdf5)

file(GLOB echelon_header_files ../echelon/*.hpp ../echelon/std/*.hpp ../echelon/detail/*.hpp ../echelon/utility/*.hpp)
file(GLOB echelon_source_files ./*.cpp)

if(NOT ${ECHELON_BUILD_STATIC})
    add_library(echelon SHARED ${echelon_header_files} ${echelon_source_files})
else()
    add_library(echelon STATIC ${echelon_header_files} ${echelon_source_files})
endif()

generate_export_header(echelon)

target_include_directories(echelon PUBLIC ${Boost_INCLUDE_DIRS} $<BUILD_INTERFACE:${ECHELON_ROOT_DIR}>)
target_compile_options(echelon PUBLIC -std=c++11 PRIVATE -pedantic -Wall -Wextra)
target_link_libraries(echelon PUBLIC echelon_hdf5 ${Boost_LIBRARIES})

set_property(TARGET echelon PROPERTY VERSION 0.7.0)
set_property(TARGET echelon PROPERTY SOVERSION 0)
set_property(TARGET echelon PROPERTY INTERFACE_echelon_MAJOR_VERSION 0)
set_property(TARGET echelon PROPERTY INTERFACE_echelon_MINOR_VERSION 7)
set_property(TARGET echelon APPEND PROPERTY COMPATIBLE_INTERFACE_STRING echelon_MAJOR_VERSION)
set_property(TARGET echelon APPEND PROPERTY COMPATIBLE_INTERFACE_STRING echelon_MINOR_VERSION)

if(NOT ECHELON_IS_A_SUBPROJECT)

    install(DIRECTORY ../echelon/ DESTINATION include/echelon
            FILES_MATCHING PATTERN "*.hpp")

endif()

if(NOT ECHELON_IS_A_SUBPROJECT)
    install(TARGETS echelon EXPORT echelon-targets
                            RUNTIME DESTINATION bin
                            INCLUDES DESTINATION include
                            LIBRARY DESTINATION lib
                            ARCHIVE DESTINATION lib)
else()
    install(TARGETS echelon OPTIONAL EXPORT echelon-targets
                            RUNTIME DESTINATION bin
                            INCLUDES DESTINATION include
                            LIBRARY DESTINATION lib
                            ARCHIVE DESTINATION lib)
                           
    add_library(echelon::echelon ALIAS echelon)
endif()

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/echelon/echelon-config-version.cmake"
  VERSION 0.7.0
  COMPATIBILITY AnyNewerVersion
)

# This feature is only supported by CMake versions >= 3.0.0.
# We should reenable it, when we bump the minimum version.
#export(EXPORT echelon-targets
#  FILE "${CMAKE_CURRENT_BINARY_DIR}/echelon/echelon-targets.cmake"
#  NAMESPACE echelon::
#)

configure_file(../cmake/echelon-config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/echelon/echelon-config.cmake"
  COPY_ONLY
)

set(ConfigPackageLocation lib/cmake/echelon)

install(EXPORT echelon-targets
  FILE
    echelon-targets.cmake
  NAMESPACE
    echelon::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    ../cmake/echelon-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/echelon/echelon-config-version.cmake"
  DESTINATION
    ${ConfigPackageLocation})
