#include <echelon/hdf5/dataset.hpp>

#include <echelon/hdf5/object_reference.hpp>

#include <utility>
#include <algorithm>
#include <vector>

namespace echelon
{
namespace hdf5
{

dataset::dataset(hid_t dataset_id_)
: dataset_id_(dataset_id_)
{
    if(id() > -1)
        H5Iinc_ref(dataset_id_);
}

dataset::dataset(hid_t loc_id, const std::string& name, const type& dtype,
                 const dataspace& space, const property_list& lcpl,
                 const property_list& dcpl,
                 const property_list& dapl)
:
dataset_id_(H5Dcreate2(loc_id, name.c_str(), dtype.id(), space.id(), lcpl.id(), dcpl.id(),
                       dapl.id()))
{
}

dataset::dataset(hid_t loc_id, const std::string& name,
                 const property_list& dapl)
: dataset_id_(H5Dopen2(loc_id, name.c_str(), dapl.id()))
{
}

dataset::~dataset()
{
    if (id() > -1)
        H5Dclose(id());
}

dataset::dataset(const dataset& other)
:dataset_id_(other.dataset_id_)
{
    H5Iinc_ref(dataset_id_);
}

dataset::dataset(dataset&& other)
:dataset_id_(other.dataset_id_)
{
    other.dataset_id_ = -1;
}

dataset& dataset::operator=(const dataset& other)
{
    using std::swap;

    dataset temp(other);
    swap(*this,temp);

    return *this;
}

dataset& dataset::operator=(dataset&& other)
{
    using std::swap;

    swap(dataset_id_,other.dataset_id_);

    return *this;
}

void dataset::write(const type& mem_type, const dataspace& mem_space,
                    const dataspace& file_space,
                    const property_list& xfer_plist,
                    const void* buf)
{
    H5Dwrite(id(), mem_type.id(), mem_space.id(), file_space.id(),
             xfer_plist.id(), buf);
}

void dataset::read(const type& mem_type, const dataspace& mem_space,
                   const dataspace& file_space,
                   const property_list& xfer_plist,
                   void* buf)const
{
    H5Dread(id(), mem_type.id(), mem_space.id(), file_space.id(),
            xfer_plist.id(), buf);
}

void dataset::write(const void* value)
{
    type datatype = get_type();
    dataspace space = get_space();

    write(datatype,space,space,default_property_list,value);
}

void dataset::read(void* value)const
{
    type datatype = get_type();
    dataspace space = get_space();

    read(datatype,space,space,default_property_list,value);
}

void dataset::set_extent(const std::vector<hsize_t>& dims)
{
    H5Dset_extent(id(), dims.data());
}

type dataset::get_type() const
{
    return type(H5Dget_type(id()));
}

dataspace dataset::get_space() const
{
    return dataspace(H5Dget_space(id()));
}

hid_t dataset::id() const
{
    return dataset_id_;
}

}
}
