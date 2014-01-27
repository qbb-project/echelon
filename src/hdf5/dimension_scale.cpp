//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/dimension_scale.hpp>

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/file.hpp>
#include <echelon/hdf5/type.hpp>

namespace echelon
{
namespace hdf5
{
dimension_scale::dimension_scale(object location, const std::string& dataset_name,
                                 const type& datatype, const std::vector<hsize_t>& extent,
                                 const std::string& scale_name)
{
    hdf5::precursor::dataspace space(extent);

    dim_scale_ = hdf5::precursor::dimension_scale(location.native_handle().id(), dataset_name,
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
}
