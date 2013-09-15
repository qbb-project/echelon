//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/object.hpp>

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/error_handling.hpp>

#include <cassert>
#include <utility>
#include <vector>

namespace echelon
{

namespace hdf5
{

object::object() : object_id_(-1)
{
}

object::object(hid_t object_id_) : object_id_(object_id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0,
                       "invalid object ID");
}

object::object(hid_t object_id_, share_ownership_t) : object_id_(object_id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0,
                       "invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,
                       "unable to increment the reference count");
}

object::object(hid_t loc_id_, const std::string& name)
: object_id_(H5Oopen(loc_id_, name.c_str(), H5P_DEFAULT))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

object::object(const dataset& other) : object_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,
                       "unable to increment the reference count");
}

object::object(const group& other) : object_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,
                       "unable to increment the reference count");
}

object::object(const object& other) : object_id_(other.object_id_)
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,
                       "unable to increment the reference count");
}

object::object(object&& other) : object_id_(other.object_id_)
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    other.object_id_ = -1;
}

object::~object()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Oclose(id()) >= 0, "unable to close the object");
    }
}

object& object::operator=(const object& other)
{
    using std::swap;

    object temp(other);
    std::swap(*this, temp);

    return *this;
}

object& object::operator=(object&& other)
{
    using std::swap;

    swap(object_id_, other.object_id_);

    return *this;
}

hid_t object::id() const
{
    return object_id_;
}

bool exists(const object& loc, const std::string& name)
{
    ECHELON_ASSERT_MSG(H5Iis_valid(loc.id()) > 0, "invalid object ID");

    htri_t result = H5Lexists(loc.id(), name.c_str(), H5P_DEFAULT);

    if (result < 0)
        throw_on_hdf5_error();

    if (result == 0)
        return false;

    htri_t result2 = H5Oexists_by_name(loc.id(), name.c_str(), H5P_DEFAULT);

    if (result2 < 0)
        throw_on_hdf5_error();

    return result2 > 0 ? true : false;
}

std::string get_name(const object& loc)
{
    ssize_t len = H5Iget_name(loc.id(), nullptr, 0);

    std::vector<char> name(len + 1);

    H5Iget_name(loc.id(), name.data(), len + 1);

    return std::string(name.data());
}
}
}
