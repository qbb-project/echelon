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
        return object_type::dataset;
}

object_type get_object_type_by_name(hid_t group_id, const char* name)
{
    H5O_info_t obj_info;
    H5Oget_info_by_name(group_id, name, &obj_info, H5P_DEFAULT);

    if (obj_info.type == H5O_TYPE_GROUP)
        return object_type::group;
    else if (obj_info.type == H5O_TYPE_DATASET)
        return object_type::dataset;
}

}
