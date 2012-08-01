#ifndef ECHELON_RAII_OBJECT_REFERENCE_HPP
#define ECHELON_RAII_OBJECT_REFERENCE_HPP

#include <hdf5.h>
#include <string>

namespace echelon
{
namespace hdf5
{

class dataset;

class object_reference
{
public:
    object_reference() = default;
    explicit object_reference(hid_t obj_id_);
    explicit object_reference(hobj_ref_t obj_ref_);
    object_reference(hid_t loc_id_, const std::string& name_);

    const hobj_ref_t& native_ref() const;

    H5O_type_t get_object_type(hid_t valid_obj_id) const;

    hid_t dereference(hid_t valid_obj_id)const;
private:
    hobj_ref_t obj_ref_;
};

}
}

#endif
