//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/dataset.hpp>

#include <echelon/attribute.hpp>

#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

namespace echelon
{

dataset::dataset(const object& parent, const std::string& name,
        const type& datatype, const std::vector<hsize_t>& shape_,
        int comp_level)
:dataset_wrapper_(-1),attributes(*this),dimensions(*this,shape_.size())
{
    hdf5::property_list dataset_creation_properties(
             hdf5::property_list_class(H5P_DATASET_CREATE));

    if(comp_level > -1)
    {
        std::vector<hsize_t> chunk_size(shape_.size(),100);

        dataset_creation_properties.set_chunk(chunk_size);
        dataset_creation_properties.set_deflate(comp_level);
    }


    dataset_wrapper_ = hdf5::dataset(parent.id(),name,datatype.get_native_type(),
                        hdf5::dataspace(shape_),hdf5::default_property_list,
                        dataset_creation_properties,hdf5::default_property_list);
}

dataset::dataset(hdf5::dataset dataset_wrapper_)
:dataset_wrapper_(dataset_wrapper_),attributes(*this),dimensions(*this,rank())
{
}

std::vector<hsize_t> dataset::shape()const
{
    return dataset_wrapper_.get_space().get_simple_extent_dims();
}

std::size_t dataset::rank()const
{
    return dataset_wrapper_.get_space().get_simple_extent_ndims();
}

type dataset::datatype()const
{
    return type(dataset_wrapper_.datatype());
}

object_reference dataset::ref()const
{
    return object_reference(object(*this));
}

hid_t dataset::id()const
{
    return dataset_wrapper_.id();
}

const hdf5::dataset& dataset::get_native_handle()const
{
    return dataset_wrapper_;
}

hdf5::dataset& dataset::get_native_handle()
{
    return dataset_wrapper_;
}

}
