//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/type.hpp>
#include <echelon/hdf5/error_handling.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{

type::type(hid_t type_id_, bool is_transient_)
: type_id_(type_id_), is_transient_(is_transient_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0 || !is_transient_,
                       "invalid object ID");
}

type::~type()
{
    if (is_transient_ && id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Tclose(id()) >= 0, "unable to close the type");
    }
}

type::type(const type& other)
: type_id_(other.id()), is_transient_(other.is_transient_)
{
    if (is_transient_)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,
                           "unable to increment the reference count");
    }
}

type::type(type&& other)
: type_id_(other.id()), is_transient_(other.is_transient_)
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) || !is_transient_,
                       "invalid object ID");

    other.type_id_ = -1;
}

type& type::operator=(const type& other)
{
    using std::swap;

    type temp(other);
    swap(*this, temp);

    return *this;
}

type& type::operator=(type&& other)
{
    using std::swap;

    swap(type_id_, other.type_id_);
    swap(is_transient_, other.is_transient_);

    return *this;
}

std::size_t type::size() const
{
    std::size_t size = H5Tget_size(id());

    if (size == 0)
        throw_on_hdf5_error();

    return size;
}

type type::char_()
{
    return type(H5T_NATIVE_CHAR);
}

type type::uchar()
{
    return type(H5T_NATIVE_UCHAR);
}

type type::int_()
{
    return type(H5T_NATIVE_INT);
}

type type::uint()
{
    return type(H5T_NATIVE_UINT);
}

type type::short_()
{
    return type(H5T_NATIVE_SHORT);
}

type type::ushort()
{
    return type(H5T_NATIVE_USHORT);
}

type type::long_()
{
    return type(H5T_NATIVE_LONG);
}

type type::ulong()
{
    return type(H5T_NATIVE_ULONG);
}

type type::long_long()
{
    return type(H5T_NATIVE_LLONG);
}

type type::ulong_long()
{
    return type(H5T_NATIVE_ULLONG);
}

type type::float_()
{
    return type(H5T_NATIVE_FLOAT);
}

type type::double_()
{
    return type(H5T_NATIVE_DOUBLE);
}

type type::string()
{
    hid_t tid = H5Tcopy(H5T_C_S1);

    if (tid < 0)
        throw_on_hdf5_error();

    if (H5Tset_cset(tid, H5T_CSET_UTF8) < 0)
        throw_on_hdf5_error();

    if (H5Tset_size(tid, H5T_VARIABLE) < 0)
        throw_on_hdf5_error();

    return type(tid);
}

type type::empty_compound_type(std::size_t size)
{
    hid_t tid = H5Tcreate(H5T_COMPOUND, size);

    if (tid < 0)
        throw_on_hdf5_error();

    return type(tid);
}

type type::object_reference()
{
    return type(H5T_STD_REF_OBJ);
}

void type::insert_member(const std::string& name, std::size_t offset,
                         const type& field_type)
{
    herr_t error_code = H5Tinsert(id(), name.c_str(), offset, field_type.id());

    if (error_code < 0)
        throw_on_hdf5_error();
}

type type::clone() const
{
    hid_t tid = H5Tcopy(id());

    if (tid < 0)
        throw_on_hdf5_error();

    return type(tid, is_transient_);
}

hid_t type::id() const
{
    return type_id_;
}

bool operator==(const type& lhs, const type& rhs)
{
    htri_t result = H5Tequal(lhs.id(), rhs.id());

    if (result < 0)
        throw_on_hdf5_error();

    return result > 0 ? true : false;
}

bool operator!=(const type& lhs, const type& rhs)
{
    return !(lhs == rhs);
}
}
}
