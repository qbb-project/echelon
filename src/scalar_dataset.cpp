//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/scalar_dataset.hpp>

#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

namespace echelon
{

scalar_dataset::scalar_dataset(native_handle_type native_handle_)
: group_handle_{std::move(native_handle_)},
  scalar_dataset_handle_{hdf5::scalar_dataset(group_handle_["data"])}
{
}

type scalar_dataset::datatype() const
{
    return type(scalar_dataset_handle_.datatype());
}

object_reference scalar_dataset::ref() const
{
    return object_reference(group_handle_.ref());
}

scalar_dataset::native_handle_type scalar_dataset::native_handle() const
{
    return group_handle_;
}

scalar_dataset::operator bool() const
{
    return static_cast<bool>(native_handle());
}

attribute_repository<scalar_dataset> scalar_dataset::attributes() const
{
    return attribute_repository<scalar_dataset>(native_handle());
}
}
