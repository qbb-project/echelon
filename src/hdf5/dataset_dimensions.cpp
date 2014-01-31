//  Copyright (c) 2013-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/dataset_dimensions.hpp>
#include <echelon/hdf5/dataset.hpp>

#include <utility>
#include <cassert>

namespace echelon
{
namespace hdf5
{
dimension::dimension(const dataset& associated_dataset_, std::size_t index_)
: associated_dataset_(&associated_dataset_), index_(index_)
{
}

dimension_scale dimension::attach_dimension_scale(const std::string& scale_name, object location,
                                                  const std::string& dataset_name,
                                                  const type& datatype,
                                                  const std::vector<hsize_t>& extent)
{
    dimension_scale dim_scale(location, dataset_name, datatype, extent, scale_name);

    hdf5::precursor::attach_dimension_scale(dim_scale.native_handle(),
                                            associated_dataset_->native_handle(), index_);

    return dim_scale;
}

std::string dimension::label() const
{
    return associated_dataset_->native_handle().label(index_);
}

void dimension::relabel(const std::string& new_label)
{
    associated_dataset_->native_handle().relabel(index_, new_label);
}

hsize_t dimension::extend() const
{
    return associated_dataset_->native_handle().get_space().get_simple_extent_dims()[index_];
}

dataset_dimensions::dataset_dimensions(const dataset& associated_dataset_, std::size_t rank_)
{
    for (std::size_t i = 0; i < rank_; ++i)
        dimensions_.emplace_back(associated_dataset_, i);

    assert(rank_ == dimensions_.size());
}

std::size_t dataset_dimensions::count() const
{
    return dimensions_.size();
}
}
}
