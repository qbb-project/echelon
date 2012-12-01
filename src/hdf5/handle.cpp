//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/handle.hpp>

#include <utility>

#include <echelon/hdf5/attribute.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/file.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/object.hpp>
#include <echelon/hdf5/property_list.hpp>
#include <echelon/hdf5/type.hpp>

#include <echelon/hdf5/error_handling.hpp>

namespace echelon
{

namespace hdf5
{

handle::handle()
:id_(-1)
{
}

handle::handle(hid_t id_)
:id_(id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0,
                       "invalid object ID");
}

handle::handle(const attribute& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const dataset& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const dataspace& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const file& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const group& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const object& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const property_list& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const type& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(const handle& other)
:id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

handle::handle(handle&& other)
:id_(other.id_)
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    other.id_ = -1;
}

handle::~handle()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

        ECHELON_VERIFY_MSG(H5Idec_ref(id()) >= 0,"unable to close the object");
    }
}

handle& handle::operator=(const handle& other)
{
    handle temp(other);

    std::swap(*this,temp);

    return *this;
}

handle& handle::operator=(handle&& other)
{
    std::swap(id_,other.id_);

    return *this;
}

hid_t handle::id()const
{
    return id_;
}

}

}
