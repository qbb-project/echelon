//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/object_reference.hpp>

#include <echelon/object.hpp>
#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

#include <utility>

namespace echelon
{

object_reference::object_reference() : reference_handle_{}
{
}

object_reference::object_reference(hdf5::object_reference native_handle_)
: reference_handle_{std::move(native_handle_)}
{
}

object object_reference::operator*() const
{
    return object(*reference_handle_);
}

object_reference::operator bool() const
{
    return static_cast<bool>(reference_handle_);
}
}
