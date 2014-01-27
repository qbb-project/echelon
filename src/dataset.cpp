//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/dataset.hpp>

#include <echelon/attribute.hpp>

#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

#include <cmath>
#include <utility>

namespace echelon
{

dataset::dataset(native_handle_type native_handle_)
: group_handle_{std::move(native_handle_)}, dataset_handle_{group_handle_["data"]}
{
}

std::vector<hsize_t> dataset::shape() const
{
    return dataset_handle_.shape();
}

std::size_t dataset::rank() const
{
    return dataset_handle_.rank();
}

type dataset::datatype() const
{
    return type(dataset_handle_.datatype());
}

object_reference dataset::ref() const
{
    return object_reference(dataset_handle_.ref());
}

dataset::native_handle_type dataset::native_handle() const
{
    return group_handle_;
}

attribute_repository<dataset> dataset::attributes() const
{
    return attribute_repository<dataset>(native_handle());
}

dataset_dimensions dataset::dimensions() const
{
    return dataset_dimensions(group_handle_);
}
}
