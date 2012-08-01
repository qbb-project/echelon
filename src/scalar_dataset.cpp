#include <echelon/scalar_dataset.hpp>

namespace echelon
{

scalar_dataset::scalar_dataset(const object& parent, const std::string& name,
                               const type& datatype, const std::vector<hsize_t>& dims)
: dataset_wrapper_(parent.id(), name, datatype.get_native_type(),
                   hdf5::dataspace(dims), hdf5::default_property_list,
                   hdf5::default_property_list, hdf5::default_property_list)
{
}

hid_t scalar_dataset::id()const
{
    return dataset_wrapper_.id();
}

}
