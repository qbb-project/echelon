#  Copyright (c) 2012-2013 Christopher Hinz
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

macro(add_imported_target lib_path)

get_filename_component(target_name ${lib_path} NAME_WE)
get_filename_component(target_ext ${lib_path} EXT)

if(NOT TARGET imported_target_${target_name})

if(${target_ext} MATCHES ".so")

  add_library(imported_target_${target_name} SHARED IMPORTED)

else(${target_ext} MATCHES ".so")

 if(${target_ext} MATCHES ".a")
   
  add_library(imported_target_${target_name} STATIC IMPORTED)

 else(${target_ext} MATCHES ".a")

   message(SEND_ERROR "${target_ext} is not a valid library extension")

 endif(${target_ext} MATCHES ".a")

endif(${target_ext} MATCHES ".so")

set_property(TARGET imported_target_${target_name} PROPERTY IMPORTED_LOCATION ${lib_path})

list(APPEND imported_target_list_proxy imported_target_${target_name})

else()

#some sanity checks
get_property(imported_location TARGET imported_target_${target_name} PROPERTY IMPORTED_LOCATION)

if(NOT ${imported_location} STREQUAL ${lib_path})

  message(SEND_ERROR "sanity check failed: library locations differ")

endif()

get_property(imported TARGET imported_target_${target_name} PROPERTY IMPORTED)

if(NOT imported)

  message(SEND_ERROR "sanity check failed: target is not imported")

endif()


list(APPEND imported_target_list_proxy imported_target_${target_name})

endif()

endmacro(add_imported_target)


function(generate_imported_library_targets library_paths imported_target_list)

foreach(lib_path ${${library_paths}})

if(lib_path)

    if((${lib_path} STREQUAL "debug") OR (${lib_path} STREQUAL "optimized") OR (${lib_path} STREQUAL "general")
    OR (${lib_path} STREQUAL "LINK_PRIVATE") OR (${lib_path} STREQUAL "LINK_PUBLIC"))

        set(previous_library_type_specifier ${lib_path})

    elseif(${lib_path} MATCHES "^-[^ \\f\\n\\r\\t\\v]+$")

        if(previous_library_type_specifier)

            list(APPEND imported_target_list_proxy ${previous_library_type_specifier})
            set(previous_library_type_specifier NO)

        endif() 

        list(APPEND imported_target_list_proxy ${lib_path})

    else()

        if(previous_library_type_specifier)

            list(APPEND imported_target_list_proxy ${previous_library_type_specifier})
            set(previous_library_type_specifier NO)         

        endif()

        add_imported_target(${lib_path})

    endif()

endif()

endforeach()

set(${imported_target_list} ${imported_target_list_proxy} PARENT_SCOPE)

endfunction(generate_imported_library_targets) 
