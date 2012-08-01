#include <echelon/dataset.hpp>

#include <echelon/attribute.hpp>

#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

namespace echelon
{

dataset::dataset(const object& parent, const std::string& name,
        const type& datatype, const std::vector<hsize_t>& dims)
:dataset_wrapper_(parent.id(),name,datatype.get_native_type(),
     hdf5::dataspace(dims),hdf5::default_property_list,
     hdf5::default_property_list,hdf5::default_property_list),
 attributes(*this)
{
}

dataset::dataset(hdf5::dataset dataset_wrapper_)
:dataset_wrapper_(dataset_wrapper_),attributes(*this)
{
}

object_reference dataset::ref()const
{
    return object_reference(*this);
}

hid_t dataset::id()const
{
    return dataset_wrapper_.id();
}

}
