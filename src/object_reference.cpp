//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/object_reference.hpp>

#include <echelon/object.hpp>
#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

namespace echelon
{

object_reference::object_reference() : reference_wrapper_(), any_valid_handle_()
{
}

object_reference::object_reference(const object& referenced_object)
: reference_wrapper_(referenced_object.id()),
  any_valid_handle_(referenced_object.get_native_handle())
{
}

object_reference::object_reference(hdf5::object_reference reference_wrapper_,
                                   hdf5::handle any_valid_handle_)
: reference_wrapper_(std::move(reference_wrapper_)),
  any_valid_handle_(std::move(any_valid_handle_))
{
}

object object_reference::operator*() const
{
    return object(
        hdf5::object(reference_wrapper_.dereference(any_valid_handle_.id())));
}

object_reference::operator bool() const
{
    return static_cast<bool>(reference_wrapper_);
}
}
