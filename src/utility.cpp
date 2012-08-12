#include <echelon/utility.hpp>

namespace echelon
{

object_type get_object_type(hid_t obj_id)
{
    H5O_info_t obj_info;
    H5Oget_info(obj_id, &obj_info);

    if (obj_info.type == H5O_TYPE_GROUP)
        return object_type::group;
    else if (obj_info.type == H5O_TYPE_DATASET)
    {
        if(H5Sget_simple_extent_type(H5Dget_space(obj_id)) == H5S_SCALAR)
            return object_type::scalar_dataset;
        else
            return object_type::dataset;
    }
}

}
