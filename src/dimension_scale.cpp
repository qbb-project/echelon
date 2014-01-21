//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/dimension_scale.hpp>

#include <echelon/dataset.hpp>
#include <echelon/file.hpp>
#include <echelon/type.hpp>

namespace echelon
{

dimension_scale::dimension_scale(const dataset& associated_dataset,
                                 const std::string& dataset_name,
                                 const type& datatype,
                                 const std::vector<hsize_t>& extent,
                                 const std::string& scale_name)
{
    hdf5::group dimensions_scales(associated_dataset.native_handle().id(),"dimension_scales",
                                  hdf5::default_property_list);

    hdf5::dataspace space(extent);

    auto enclosing_group = hdf5::group(dimensions_scales.id(), dataset_name, hdf5::default_property_list, hdf5::default_property_list, hdf5::default_property_list);
    
    dim_scale_ =
        hdf5::dimension_scale(enclosing_group.id(), "scale",
                              datatype.native_handle(), space, scale_name);
}

std::vector<hsize_t> dimension_scale::shape() const
{
    return dim_scale_.get_space().get_simple_extent_dims();
}

const dimension_scale::native_handle_type& dimension_scale::native_handle() const
{
    return dim_scale_;
}
}
