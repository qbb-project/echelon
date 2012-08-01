#ifndef ECHELON_UTILITY_HPP
#define ECHELON_UTILITY_HPP

#include <hdf5.h> //for hid_t

namespace echelon
{

enum class object_type
{
    group,
    dataset
};

object_type get_object_type(hid_t obj_id);
object_type get_object_type_by_name(hid_t group_id, const char* name);

}

#endif
