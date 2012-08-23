#include <echelon/hdf5/object_reference.hpp>

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/handle.hpp>

#include <vector>
#include <type_traits>

namespace echelon
{
namespace hdf5
{

static_assert(std::is_pod<object_reference>::value,
              "hdf5::object_reference should be a POD type");
static_assert(sizeof(object_reference) == sizeof(hobj_ref_t),
              "hdf5::object_reference should have the same size as hobj_ref_t");

object_reference::object_reference(hid_t obj_id_)
{
    std::size_t len = H5Iget_name(obj_id_,0,0);

    std::vector<char> name(len + 1);
    H5Iget_name(obj_id_,name.data(),len + 1);

    hdf5::handle file_handle(H5Iget_file_id(obj_id_));

    H5Rcreate(&obj_ref_, file_handle.id(), name.data(), H5R_OBJECT, -1);
}

object_reference::object_reference(hid_t loc_id_, const std::string& name_)
{
    H5Rcreate(&obj_ref_, loc_id_, name_.c_str(), H5R_OBJECT, -1);
}

object_reference::object_reference(hobj_ref_t obj_ref_)
: obj_ref_(obj_ref_)
{
}

const hobj_ref_t& object_reference::native_ref() const
{
    return obj_ref_;
}

H5O_type_t object_reference::get_object_type(hid_t valid_obj_id) const
{
    H5O_type_t obj_type;

    H5Rget_obj_type2(valid_obj_id, H5R_OBJECT, &obj_ref_, &obj_type);

    return obj_type;
}

hid_t object_reference::dereference(hid_t valid_obj_id)const
{
    hid_t ref_obj_id = H5Rdereference(valid_obj_id, H5R_OBJECT, &obj_ref_);

    return ref_obj_id;
}

}
}
