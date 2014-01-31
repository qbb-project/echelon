//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/object_reference.hpp>

#include <echelon/hdf5/object.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/dataset.hpp>

namespace echelon
{
namespace hdf5
{
object_reference::object_reference() : reference_wrapper_(), any_valid_handle_()
{
}

object_reference::object_reference(const object& referenced_object)
: reference_wrapper_(referenced_object.native_handle().id()),
  any_valid_handle_(referenced_object.native_handle())
{
}

object_reference::object_reference(hdf5::precursor::object_reference reference_wrapper_,
                                   hdf5::precursor::handle any_valid_handle_)
: reference_wrapper_(std::move(reference_wrapper_)), any_valid_handle_(std::move(any_valid_handle_))
{
}

object object_reference::operator*() const
{
    return object(hdf5::precursor::object(reference_wrapper_.dereference(any_valid_handle_.id())));
}

object_reference::operator bool() const
{
    return static_cast<bool>(reference_wrapper_);
}
}
}
