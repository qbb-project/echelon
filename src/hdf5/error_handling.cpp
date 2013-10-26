//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/error_handling.hpp>

#include <exception>
#include <map>

#include <utility>
#include <functional>
#include <cassert>

namespace echelon
{
namespace hdf5
{

namespace
{

template <typename E>
struct error_translator
{
    void operator()(const std::exception_ptr& current_exception,
                    const H5E_error2_t* err_desc) const
    {
        if (current_exception)
        {
            try
            {
                std::rethrow_exception(current_exception);
            }
            catch (...)
            {
                std::throw_with_nested(E(err_desc->desc, err_desc->min_num));
            }
        }
        else
        {
            throw E(err_desc->desc, err_desc->min_num);
        }
    }
};

std::map<hid_t,
         std::function<void(const std::exception_ptr&, const H5E_error2_t*)>>
minor_error_translation_map = {
    {H5E_NOTFOUND, error_translator<not_found_exception>()},
    {H5E_CANTOPENOBJ, error_translator<can_not_open_object_exception>()},
    {H5E_EXISTS, error_translator<exists_exception>()},
    {H5E_ALREADYEXISTS, error_translator<already_exists_exception>()},
    {H5E_CANTOPENFILE, error_translator<cant_open_file_exception>()},
    {H5E_CANTCREATE, error_translator<cant_open_file_exception>()},
    {H5E_NOTHDF5, error_translator<invalid_hdf5_file_exception>()},
    {H5E_UNSUPPORTED, error_translator<unsupported_feature_exception>()},
    {H5E_BADRANGE, error_translator<out_of_range_exception>()},
    {H5E_BADVALUE, error_translator<bad_value_exception>()},
    {H5E_BADTYPE, error_translator<invalid_type_exception>()}
};

std::map<std::pair<hid_t, hid_t>,
         std::function<void(const std::exception_ptr&, const H5E_error2_t*)>>
overwrite_error_translation_map = {
    {{H5E_SYM, H5E_CANTINIT},
     error_translator<symbol_already_exists_exception>()}};

std::map<hid_t,
         std::function<void(const std::exception_ptr&, const H5E_error2_t*)>>
error_translation_map = {
    {H5E_DATASET, error_translator<dataset_exception>()},
    {H5E_FUNC, error_translator<function_entry_exit_exception>()},
    {H5E_STORAGE, error_translator<storage_exception>()},
    {H5E_FILE, error_translator<file_exception>()},
    {H5E_SOHM, error_translator<shared_object_header_message_exception>()},
    {H5E_SYM, error_translator<symbol_table_exception>()},
    {H5E_VFL, error_translator<virtual_file_layer_exception>()},
    {H5E_INTERNAL, error_translator<internal_exception>()},
    {H5E_BTREE, error_translator<Btree_exception>()},
    {H5E_REFERENCE, error_translator<reference_exception>()},
    {H5E_DATASPACE, error_translator<dataspace_exception>()},
    {H5E_RESOURCE, error_translator<resource_exception>()},
    {H5E_PLIST, error_translator<property_list_exception>()},
    {H5E_LINK, error_translator<link_exception>()},
    {H5E_DATATYPE, error_translator<type_exception>()},
    {H5E_RS, error_translator<reference_counted_string_exception>()},
    {H5E_HEAP, error_translator<heap_exception>()},
    {H5E_OHDR, error_translator<object_header_exception>()},
    {H5E_ATOM, error_translator<atom_exception>()},
    {H5E_ATTR, error_translator<attribute_exception>()},
    {H5E_IO, error_translator<io_exception>()},
    {H5E_SLIST, error_translator<skip_list_exception>()},
    {H5E_EFL, error_translator<external_file_list_exception>()},
    {H5E_TST, error_translator<ternary_search_tree_exception>()},
    {H5E_ARGS, error_translator<invalid_argument_exception>()},
    {H5E_ERROR, error_translator<error_exception>()},
    {H5E_PLINE, error_translator<data_filter_exception>()},
    {H5E_FSPACE, error_translator<free_space_manager_exception>()},
    {H5E_CACHE, error_translator<metadata_cache_exception>()}};

extern "C" {

static herr_t kubus_hdf5_translate_error_stack(unsigned,
                                               const H5E_error2_t* err_desc,
                                               void* client_data)
{
    std::exception_ptr& current_exception =
        *static_cast<std::exception_ptr*>(client_data);

    try
    {
        auto iter = overwrite_error_translation_map.find(
            std::make_pair(err_desc->maj_num, err_desc->min_num));

        if (iter != end(overwrite_error_translation_map))
        {
            iter->second(current_exception, err_desc);
        }
        else
        {
            auto iter = minor_error_translation_map.find(err_desc->min_num);

            if (iter != end(minor_error_translation_map))
            {
                iter->second(current_exception, err_desc);
            }
            else
            {
                auto iter = error_translation_map.find(err_desc->maj_num);

                ECHELON_ASSERT_MSG(iter != end(error_translation_map),
                                   "no valid error translation found");

                iter->second(current_exception, err_desc);
            }
        }
    }
    catch (...)
    {
        current_exception = std::current_exception();
    }

    return 0;
}
}

struct error_handling_enabler
{
    error_handling_enabler()
    {
        enable_error_handling();
    }
};

error_handling_enabler eh_enabler = error_handling_enabler();
}

void throw_on_hdf5_error()
{
    std::exception_ptr nested_exception;

    herr_t result =
        H5Ewalk2(H5E_DEFAULT, H5E_WALK_UPWARD,
                 &kubus_hdf5_translate_error_stack, &nested_exception);

    if (result < 0)
        throw_on_hdf5_error();

    if (nested_exception)
        std::rethrow_exception(nested_exception);
}

void enable_error_handling()
{
    H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);
}
}
}
