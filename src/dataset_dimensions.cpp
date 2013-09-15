//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/dataset_dimensions.hpp>
#include <cassert>

#include <echelon/dataset.hpp>

namespace echelon
{

dimension::dimension(dataset& associated_dataset_, std::size_t index_)
: associated_dataset_(&associated_dataset_), index_(index_)
{
}

dimension_scale dimension::attach_dimension_scale(const std::string& name,
                                                  const type& datatype)
{
    std::vector<hsize_t> extent = {extend()};

    std::string dataset_name = "dim" + std::to_string(index_);

    dimension_scale dim_scale(*associated_dataset_, dataset_name, datatype,
                              extent, name);

    hdf5::attach_dimension_scale(dim_scale.get_native_handle(),
                                 associated_dataset_->get_native_handle(),
                                 index_);

    return dim_scale;
}

std::string dimension::label() const
{
    return associated_dataset_->get_native_handle().label(index_);
}

void dimension::relabel(const std::string& new_label)
{
    associated_dataset_->get_native_handle().relabel(index_, new_label);
}

hsize_t dimension::extend() const
{
    return associated_dataset_->get_native_handle()
        .get_space()
        .get_simple_extent_dims()[index_];
}

dataset_dimensions::dataset_dimensions(dataset& associated_dataset_,
                                       std::size_t rank_)
{
    for (std::size_t i = 0; i < rank_; ++i)
        dimensions_.emplace_back(associated_dataset_, i);

    assert(rank_ == dimensions_.size());
}
}
