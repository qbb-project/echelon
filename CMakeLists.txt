#  Copyright (c) 2012-2016 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required (VERSION 2.8.12)
enable_testing()

project(echelon)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "echelon is a HDF5-based library for the structured storage of scientific data.")
SET(CPACK_PACKAGE_VENDOR "Christopher Hinz")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Readme.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE_1_0.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "8")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-src")
set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_CURRENT_BINARY_DIR}/;/.git/;/.kdev4/;echelon.kdev4;~$;${CPACK_SOURCE_IGNORE_FILES}")

include(CPack)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	set(ECHELON_IS_A_SUBPROJECT 0)
else()
	set(ECHELON_IS_A_SUBPROJECT 1)
endif()

if (NOT ECHELON_IS_A_SUBPROJECT)
	add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif() 

set(CMAKE_MODULE_PATH ${echelon_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

option(ECHELON_ENABLE_1_0_FORMAT_COMPATIBILITY "Enable backwards compatibility for the file format 1.0" ON)
mark_as_advanced(ECHELON_ENABLE_1_0_FORMAT_COMPAT)

set(ECHELON_BUILD_STATIC OFF CACHE BOOL "Build static libraries if enabled.")

set(ECHELON_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include(EnableCoverage)

add_subdirectory(src)

if(NOT ECHELON_IS_A_SUBPROJECT)

    add_subdirectory(examples)
    add_subdirectory(tests)

endif()

if(NOT ECHELON_IS_A_SUBPROJECT)

  # add a target to generate API documentation with Doxygen
  find_package(Doxygen)
  find_package(Sphinx)
	  
  if(DOXYGEN_FOUND)
	  
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)

      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html)
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/latex)
	      
      add_custom_target(api_doc
	  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
	  COMMAND 
	  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
	  COMMENT "Generating API documentation with Doxygen." VERBATIM)

      list(APPEND ECHELON_DOC_TARGETS api_doc)
    
      if(SPHINX_FOUND)
      
	if(NOT DEFINED SPHINX_THEME)
	  set(SPHINX_THEME default)
	endif()
	if(NOT DEFINED SPHINX_THEME_DIR)
	  set(SPHINX_THEME_DIR)
	endif()

	set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")

	set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")

	set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/html/user_guide")
	set(SPHINX_LATEX_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/latex/user_guide")
	configure_file(
	      "${CMAKE_CURRENT_SOURCE_DIR}/doc/user-guide/conf.py.in"
	      "${BINARY_BUILD_DIR}/conf.py"
	      @ONLY)

	add_custom_target(user_guide
	      ${SPHINX_EXECUTABLE}
	      -q -b html
	      -c "${BINARY_BUILD_DIR}"
	      -d "${SPHINX_CACHE_DIR}"
	      "${CMAKE_CURRENT_SOURCE_DIR}/doc/user-guide"
	      "${SPHINX_HTML_DIR}"
	      COMMENT "Building user guide with Sphinx."
	      DEPENDS api_doc)

	list(APPEND ECHELON_DOC_TARGETS user_guide)
	
	add_custom_target(user_guide_latex
	      ${SPHINX_EXECUTABLE}
	      -q -b latex
	      -c "${BINARY_BUILD_DIR}"
	      -d "${SPHINX_CACHE_DIR}"
	      "${CMAKE_CURRENT_SOURCE_DIR}/doc/user-guide"
	      "${SPHINX_LATEX_DIR}"
	      COMMENT "Building user guide (latex version) with Sphinx.")

      endif(SPHINX_FOUND)

  endif(DOXYGEN_FOUND)

add_custom_target(doc DEPENDS ${ECHELON_DOC_TARGETS} COMMENT "Generating the documentation.")

endif()
