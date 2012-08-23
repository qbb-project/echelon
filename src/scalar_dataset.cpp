#include <echelon/scalar_dataset.hpp>

#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

namespace echelon
{

scalar_dataset::scalar_dataset(const object& parent, const std::string& name,
                               const type& datatype)
: dataset_wrapper_(parent.id(), name, datatype.get_native_type(),
                   hdf5::dataspace(), hdf5::default_property_list,
                   hdf5::default_property_list, hdf5::default_property_list),
  attributes(*this)
{
}

scalar_dataset::scalar_dataset(hdf5::dataset dataset_wrapper_)
:dataset_wrapper_(dataset_wrapper_),
 attributes(*this)
{}

object_reference scalar_dataset::ref()const
{
    return object_reference(object(*this));
}

hid_t scalar_dataset::id()const
{
    return dataset_wrapper_.id();
}

const hdf5::dataset& scalar_dataset::get_native_handle()const
{
    return dataset_wrapper_;
}

}
