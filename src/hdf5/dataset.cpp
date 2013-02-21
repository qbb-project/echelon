//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/object_reference.hpp>
#include <echelon/hdf5/error_handling.hpp>

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
    ECHELON_ASSERT_MSG(id() == -1 || H5Iget_type(id()) == H5I_DATASET,
                       "ID does not refer to a dataset");
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0,
                       "invalid object ID");
}

dataset::dataset(const object& other)
:dataset_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_ASSERT_MSG(H5Iget_type(id()) == H5I_DATASET,
                       "ID does not refer to a dataset");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

dataset::dataset(hid_t loc_id, const std::string& name, const type& dtype,
                 const dataspace& space, const property_list& lcpl,
                 const property_list& dcpl,
                 const property_list& dapl)
:
dataset_id_(H5Dcreate2(loc_id, name.c_str(), dtype.id(), space.id(), lcpl.id(), dcpl.id(),
                       dapl.id()))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

dataset::dataset(hid_t loc_id, const std::string& name,
                 const property_list& dapl)
: dataset_id_(H5Dopen2(loc_id, name.c_str(), dapl.id()))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

dataset::~dataset()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

        ECHELON_VERIFY_MSG(H5Dclose(id()) >= 0,"unable to close the dataset");
    }
}

dataset::dataset(const dataset& other)
:dataset_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

dataset::dataset(dataset&& other)
:dataset_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

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
    if(H5Dwrite(id(), mem_type.id(), mem_space.id(), file_space.id(),
             xfer_plist.id(), buf) < 0)
        throw_on_hdf5_error();
}

void dataset::read(const type& mem_type, const dataspace& mem_space,
                   const dataspace& file_space,
                   const property_list& xfer_plist,
                   void* buf)const
{
    if(H5Dread(id(), mem_type.id(), mem_space.id(), file_space.id(),
            xfer_plist.id(), buf) < 0)
        throw_on_hdf5_error();
}

void dataset::write(const void* value)
{
    type datatype = this->datatype();
    dataspace space = get_space();

    write(datatype,space,space,default_property_list,value);
}

void dataset::read(void* value)const
{
    type datatype = this->datatype();
    dataspace space = get_space();

    read(datatype,space,space,default_property_list,value);
}

void dataset::set_extent(const std::vector<hsize_t>& dims)
{
    if(H5Dset_extent(id(), dims.data()) < 0)
        throw_on_hdf5_error();
}

dataspace dataset::get_space() const
{
    hid_t space_id = H5Dget_space(id());

    if(space_id < 0)
        throw_on_hdf5_error();

    return dataspace(space_id);
}

type dataset::datatype()const
{
    hid_t type_id = H5Dget_type(id());

    if(type_id < 0)
        throw_on_hdf5_error();

    return type(type_id,true);
}

property_list dataset::creation_property_list()const
{
    return property_list(H5Dget_create_plist(id()));
}

hid_t dataset::id() const
{
    return dataset_id_;
}

}
}
