//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/object.hpp>

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/scalar_dataset.hpp>
#include <echelon/hdf5/object_reference.hpp>

#include <echelon/hdf5/utility.hpp>

#include <utility>
#include <cassert>

namespace echelon
{
namespace hdf5
{
object::object(hdf5::precursor::object object_wrapper_)
: object_wrapper_(std::move(object_wrapper_))
{
}

object::object(const group& object_) : object_wrapper_(object_.native_handle())
{
}

object::object(const dataset& object_) : object_wrapper_(object_.native_handle())
{
}

object::object(const scalar_dataset& object_) : object_wrapper_(object_.native_handle())
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
    if (get_object_type(native_handle().id()) != object_type::group)
        throw wrong_object_type_exception("wrong object type");

    return group(hdf5::precursor::group(object_wrapper_));
}

object::operator dataset() const
{
    if (get_object_type(native_handle().id()) != object_type::dataset)
        throw wrong_object_type_exception("wrong object type");

    return dataset(hdf5::precursor::dataset(object_wrapper_));
}

object::operator scalar_dataset() const
{
    if (get_object_type(native_handle().id()) != object_type::scalar_dataset)
        throw wrong_object_type_exception("wrong object type");

    return scalar_dataset(hdf5::precursor::dataset(object_wrapper_));
}

object_reference object::ref() const
{
    return object_reference(*this);
}

const object::native_handle_type& object::native_handle() const
{
    return object_wrapper_;
}
}
}
