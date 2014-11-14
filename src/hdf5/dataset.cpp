//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/dataset.hpp>

#include <echelon/hdf5/attribute.hpp>

#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/dataset.hpp>

#include <cmath>

namespace echelon
{
namespace hdf5
{
dataset::dataset(const object& parent, const std::string& name, const type& datatype,
                 const std::vector<hsize_t>& shape, const std::vector<hsize_t>& max_dims,
                 int comp_level, bool auto_chunking, bool shuffle_filter,
                 const std::vector<hsize_t> chunk_shape)
: dataset_handle_(-1)
{
    hdf5::precursor::property_list dataset_creation_properties(
        hdf5::precursor::property_list_class(H5P_DATASET_CREATE));

    bool uses_filters = false;
    
    bool is_extendible = shape != max_dims;

    if (comp_level > -1)
    {
        dataset_creation_properties.set_deflate(comp_level);
        uses_filters = true;
    }

    if (shuffle_filter)
    {
        dataset_creation_properties.set_shuffle();
        uses_filters = true;
    }

    if (!chunk_shape.empty())
    {
        dataset_creation_properties.set_chunk(chunk_shape);
    }
    else if (uses_filters || auto_chunking || is_extendible)
    {
        // perform auto-chunking

        std::size_t dataset_rank = shape.size();

        size_t chunk_cache_size;

        hdf5::precursor::property_list dataset_access_properties(
            hdf5::precursor::property_list_class(H5P_DATASET_ACCESS));
        H5Pget_chunk_cache(dataset_access_properties.id(), nullptr, &chunk_cache_size, nullptr);

        double load_factor = 0.1;

        std::size_t chunk_size = chunk_cache_size * load_factor;

        std::size_t num_of_elements = chunk_size / datatype.size();

        std::size_t chunk_width = std::pow(num_of_elements, 1.0 / dataset_rank);

        if (chunk_width == 0)
            chunk_width = 1;

        std::vector<hsize_t> auto_chunk_shape;
        auto_chunk_shape.reserve(dataset_rank);

        for (std::size_t i = 0; i < dataset_rank; ++i)
        {
            auto_chunk_shape.push_back(max_dims[i] == unlimited || chunk_width < max_dims[i] ? chunk_width : max_dims[i]);
        }

        dataset_creation_properties.set_chunk(auto_chunk_shape);
    }

    hdf5::precursor::property_list link_creation_properties(
        hdf5::precursor::property_list_class(H5P_LINK_CREATE));
    link_creation_properties.set_char_encoding(H5T_CSET_UTF8);

    hdf5::precursor::dataspace dspace = is_extendible
                                            ? hdf5::precursor::dataspace(shape, max_dims)
                                            : hdf5::precursor::dataspace(shape);
    dataset_handle_ = hdf5::precursor::dataset(
        parent.native_handle().id(), name, datatype.native_handle(),
        dspace, link_creation_properties, dataset_creation_properties,
        hdf5::precursor::default_property_list);
}

dataset::dataset(native_handle_type native_handle_) : dataset_handle_(std::move(native_handle_))
{
}

std::vector<hsize_t> dataset::shape() const
{
    return dataset_handle_.get_space().get_simple_extent_dims();
}

std::size_t dataset::rank() const
{
    return dataset_handle_.get_space().get_simple_extent_ndims();
}

type dataset::datatype() const
{
    return type(dataset_handle_.datatype());
}

object_reference dataset::ref() const
{
    return object_reference(*this);
}

const dataset::native_handle_type& dataset::native_handle() const
{
    return dataset_handle_;
}

attribute_repository<dataset> dataset::attributes() const
{
    return attribute_repository<dataset>(*this);
}

dataset_dimensions dataset::dimensions() const
{
    return dataset_dimensions(*this, shape().size());
}

dataset::operator bool() const
{
    return static_cast<bool>(native_handle());
}
}
}
