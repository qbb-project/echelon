//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/dataset.hpp>

#include <echelon/attribute.hpp>

#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

#include <cmath>
#include <iostream>

namespace echelon
{

dataset::dataset(const object& parent, const std::string& name,
                 const type& datatype, const std::vector<hsize_t>& shape,
                 int comp_level, const std::vector<hsize_t> chunk_shape)
: native_handle_(parent.native_handle().id(), name, hdf5::default_property_list,
                 hdf5::default_property_list,hdf5::default_property_list),
  dataset_wrapper_(-1), attributes(*this), dimensions(*this, shape.size())
{  
    hdf5::property_list dataset_creation_properties(
        hdf5::property_list_class(H5P_DATASET_CREATE));

    if (!chunk_shape.empty())
    {
        dataset_creation_properties.set_chunk(chunk_shape);
    }
    else if(comp_level > -1)
    {
        //perform auto-chunking
        
        std::size_t dataset_rank = shape.size();
        
        size_t chunk_cache_size;
        
        hdf5::property_list dataset_access_properties(
            hdf5::property_list_class(H5P_DATASET_ACCESS));
        H5Pget_chunk_cache(dataset_access_properties.id(),nullptr,&chunk_cache_size,nullptr);
        
        double load_factor = 0.1;
        
        std::size_t chunk_size = chunk_cache_size*load_factor;
        
        std::size_t num_of_elements = chunk_size/datatype.size();
        
        std::size_t chunk_width = std::pow(num_of_elements,1.0/dataset_rank);
        
        if(chunk_width == 0)
            chunk_width = 1;
        
        std::cout << chunk_width << std::endl;
        
        std::vector<hsize_t> auto_chunk_shape;
        auto_chunk_shape.reserve(dataset_rank);
        
        for(std::size_t i = 0;i < dataset_rank;++i)
        {
            //FIXME: chunk_width < size of dim is only needed for fixed-sized datasets
            auto_chunk_shape.push_back(chunk_width < shape[i] ? chunk_width : shape[i]);
        }
        
        dataset_creation_properties.set_chunk(auto_chunk_shape);
    }

    if (comp_level > -1)
    {
        dataset_creation_properties.set_deflate(comp_level);
    }

    hdf5::property_list link_creation_properties(
        hdf5::property_list_class(H5P_LINK_CREATE));
    link_creation_properties.set_char_encoding(H5T_CSET_UTF8);

    dataset_wrapper_ =
        hdf5::dataset(native_handle_.id(), "data", datatype.native_handle(),
                      hdf5::dataspace(shape), link_creation_properties,
                      dataset_creation_properties, hdf5::default_property_list);
        
    hdf5::group(native_handle_.id(),"dimension_scales", hdf5::default_property_list,
                hdf5::default_property_list, hdf5::default_property_list);
}

dataset::dataset(native_handle_type native_handle_)
: native_handle_(std::move(native_handle_)),
  dataset_wrapper_(hdf5::dataset(this->native_handle_.id(),"data",hdf5::default_property_list)),
  attributes(*this),  dimensions(*this, rank())
{
}

std::vector<hsize_t> dataset::shape() const
{
    return dataset_wrapper_.get_space().get_simple_extent_dims();
}

std::size_t dataset::rank() const
{
    return dataset_wrapper_.get_space().get_simple_extent_ndims();
}

type dataset::datatype() const
{
    return type(dataset_wrapper_.datatype());
}

object_reference dataset::ref() const
{
    return object_reference(*this);
}

const dataset::native_handle_type& dataset::native_handle() const
{
    return native_handle_;
}

const hdf5::dataset& dataset::data() const
{
    return dataset_wrapper_;
}

}
