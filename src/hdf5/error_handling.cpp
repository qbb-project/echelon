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

template<typename E>
struct error_translator
{
    void operator()(const std::exception_ptr& current_exception,
                    const H5E_error2_t* err_desc)const
    {
        if(current_exception)
        {
            try
            {
                std::rethrow_exception(current_exception);
            }
            catch(...)
            {
                std::throw_with_nested( E(err_desc->desc, err_desc->min_num) );
            }
        }
        else
        {
            throw E(err_desc->desc, err_desc->min_num);
        }
    }
};

std::map<hid_t,
         std::function<void(const std::exception_ptr&,const H5E_error2_t*)> >
    error_translation_map =
    {
     { H5E_DATASET , error_translator<dataset_exception>() },
     { H5E_FUNC , error_translator<function_entry_exit_exception>() },
     { H5E_STORAGE , error_translator<storage_exception>() },
     { H5E_FILE , error_translator<file_exception>() },
     { H5E_SOHM , error_translator<shared_object_header_message_exception>() },
     { H5E_SYM , error_translator<symbol_table_exception>() },
     { H5E_VFL , error_translator<virtual_file_layer_exception>() },
     { H5E_INTERNAL , error_translator<internal_exception>() },
     { H5E_BTREE , error_translator<Btree_exception>() },
     { H5E_REFERENCE , error_translator<reference_exception>() },
     { H5E_DATASPACE , error_translator<dataspace_exception>() },
     { H5E_RESOURCE , error_translator<resource_exception>() },
     { H5E_PLIST , error_translator<property_list_exception>() },
     { H5E_LINK , error_translator<link_exception>() },
     { H5E_DATATYPE , error_translator<type_exception>() },
     { H5E_RS , error_translator<reference_counted_string_exception>() },
     { H5E_HEAP , error_translator<heap_exception>() },
     { H5E_OHDR , error_translator<object_header_exception>() },
     { H5E_ATOM , error_translator<atom_exception>() },
     { H5E_ATTR , error_translator<attribute_exception>() },
     { H5E_IO , error_translator<io_exception>() },
     { H5E_SLIST , error_translator<skip_list_exception>() },
     { H5E_EFL , error_translator<external_file_list_exception>() },
     { H5E_TST , error_translator<ternary_search_tree_exception>() },
     { H5E_ARGS , error_translator<invalid_argument_exception>() },
     { H5E_ERROR , error_translator<error_exception>() },
     { H5E_PLINE , error_translator<data_filter_exception>() },
     { H5E_FSPACE , error_translator<free_space_manager_exception>() },
     { H5E_CACHE , error_translator<metadata_cache_exception>() }
    };


herr_t translate_error_stack(unsigned, const H5E_error2_t* err_desc,void* client_data)
{
    std::exception_ptr& current_exception = *static_cast<std::exception_ptr*>(client_data);

    try
    {
        auto iter = error_translation_map.find(err_desc->maj_num);

        const bool no_valid_error_translation = iter != error_translation_map.end();
        assert(no_valid_error_translation);

        iter->second(current_exception,err_desc);
    }
    catch(...)
    {
        current_exception = std::current_exception();
    }

    return 0;
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

    herr_t result = H5Ewalk2(H5E_DEFAULT,H5E_WALK_UPWARD,
                             &translate_error_stack,&nested_exception);

    if(result < 0)
        throw_on_hdf5_error();

    if(nested_exception)
        std::rethrow_exception(nested_exception);
}

void enable_error_handling()
{
    H5Eset_auto2(H5E_DEFAULT,0,0);
}

}
}
