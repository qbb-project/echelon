#ifndef ECHELON_UTILITY_HPP
#define ECHELON_UTILITY_HPP

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

}

#endif
