//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/object.hpp>

#include <echelon/dataset.hpp>
#include <echelon/group.hpp>
#include <echelon/scalar_dataset.hpp>
#include <echelon/object_reference.hpp>

#include <utility>
#include <cassert>

namespace echelon
{

object::object(hdf5::object native_handle_) : object_handle_(std::move(native_handle_))
{
}

object::object(const group& object_) : object_handle_(object_.native_handle())
{
}

object::object(const dataset& object_) : object_handle_(object_.native_handle())
{
}

object::object(const scalar_dataset& object_) : object_handle_(object_.native_handle())
{
}

object& object::operator=(const group& object_)
{
    using std::swap;

    object temp(object_);
    std::swap(*this, temp);

    return *this;
}

object& object::operator=(const dataset& object_)
{
    using std::swap;

    object temp(object_);
    std::swap(*this, temp);

    return *this;
}

object& object::operator=(const scalar_dataset& object_)
{
    using std::swap;

    object temp(object_);
    std::swap(*this, temp);

    return *this;
}

object::operator group() const
{
    return group(object_handle_);
}

object::operator dataset() const
{
    // FIXME: add a more precise type test here

    return dataset(hdf5::group(object_handle_));
}

object::operator scalar_dataset() const
{
    return scalar_dataset(object_handle_);
}

object_reference object::ref() const
{
    return object_reference(object_handle_.ref());
}

object::native_handle_type object::native_handle() const
{
    return object_handle_;
}
}
