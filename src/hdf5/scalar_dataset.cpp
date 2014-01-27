//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/scalar_dataset.hpp>

#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/dataset.hpp>

namespace echelon
{
namespace hdf5
{
scalar_dataset::scalar_dataset(const object& parent, const std::string& name, const type& datatype)
: dataset_wrapper_(parent.native_handle().id(), name, datatype.native_handle(),
                   hdf5::precursor::dataspace(), hdf5::precursor::default_property_list,
                   hdf5::precursor::default_property_list, hdf5::precursor::default_property_list)
{
}

scalar_dataset::scalar_dataset(hdf5::precursor::dataset dataset_wrapper_)
: dataset_wrapper_(std::move(dataset_wrapper_))
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

const scalar_dataset::native_handle_type& scalar_dataset::native_handle() const
{
    return dataset_wrapper_;
}

attribute_repository<scalar_dataset> scalar_dataset::attributes() const
{
    return attribute_repository<scalar_dataset>(*this);
}
}
}
