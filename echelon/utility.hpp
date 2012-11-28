#ifndef ECHELON_UTILITY_HPP
#define ECHELON_UTILITY_HPP

#include <echelon/object.hpp>

#include <string>

#include <hdf5.h> //for hid_t

namespace echelon
{

enum class object_type
{
    group,
    dataset,
    scalar_dataset,
    unknown
};

object_type get_object_type(hid_t obj_id);

object_type get_object_type_by_name(const object& loc,const std::string& name);

bool exists(const object& loc,const std::string& name);

}

#endif
