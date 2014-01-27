//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/dataset_dimensions.hpp>
#include <echelon/dataset.hpp>

#include <cassert>
#include <utility>

namespace echelon
{

dimension::dimension(hdf5::group containing_group_handle_, std::size_t index_)
: containing_group_handle_{std::move(containing_group_handle_)}, index_{index_}
{
}

dimension_scale dimension::attach_dimension_scale(const std::string& name, const type& datatype)
{
    hdf5::dataset data = containing_group_handle_["data"];

    hdf5::group dimensions = containing_group_handle_.require_group("dimensions");

    std::string dim_name = "dimension<" + std::to_string(index_) + ">";

    hdf5::group this_dim = dimensions.require_group(dim_name);

    hdf5::group dim_scales = this_dim.require_group("dimension_scales");

    auto dim_scale = data.dimensions()[index_].attach_dimension_scale(
        name, dim_scales, name, datatype.native_handle(), {extend()});

    return dimension_scale(dim_scale);
}

std::string dimension::label() const
{
    hdf5::dataset data = containing_group_handle_["data"];

    return data.dimensions()[index_].label();
}

void dimension::relabel(const std::string& new_label)
{
    hdf5::dataset data = containing_group_handle_["data"];

    return data.dimensions()[index_].relabel(new_label);
}

hsize_t dimension::extend() const
{
    hdf5::dataset data = containing_group_handle_["data"];

    return data.dimensions()[index_].extend();
}

dataset_dimensions::dataset_dimensions(hdf5::group containing_group_handle_)
{
    hdf5::dataset data = containing_group_handle_["data"];
    auto rank = data.dimensions().count();

    for (std::size_t i = 0; i < rank; ++i)
        dimensions_.emplace_back(containing_group_handle_, i);

    assert(rank == dimensions_.size());
}
}
