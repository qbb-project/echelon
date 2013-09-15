//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/scalar_dataset.hpp>

#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

namespace echelon
{

scalar_dataset::scalar_dataset(const object& parent, const std::string& name,
                               const type& datatype)
: dataset_wrapper_(parent.id(), name, datatype.get_native_type(),
                   hdf5::dataspace(), hdf5::default_property_list,
                   hdf5::default_property_list, hdf5::default_property_list),
  attributes(*this)
{
}

scalar_dataset::scalar_dataset(hdf5::dataset dataset_wrapper_)
: dataset_wrapper_(dataset_wrapper_), attributes(*this)
{
}

type scalar_dataset::datatype() const
{
    return type(dataset_wrapper_.datatype());
}

object_reference scalar_dataset::ref() const
{
    return object_reference(*this);
}

hid_t scalar_dataset::id() const
{
    return dataset_wrapper_.id();
}

const hdf5::dataset& scalar_dataset::get_native_handle() const
{
    return dataset_wrapper_;
}
}
