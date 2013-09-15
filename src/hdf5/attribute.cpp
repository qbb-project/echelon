//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/attribute.hpp>
#include <echelon/hdf5/error_handling.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{

attribute::attribute(hid_t attribute_id_) : attribute_id_(attribute_id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iget_type(id()) == H5I_ATTR,
                       "ID does not refer to a attribute");
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0,
                       "invalid object ID");
}

attribute::attribute(hid_t loc_id_, const std::string& name_,
                     const type& attr_type_, const property_list& acpl,
                     const property_list& aapl)
: attribute_id_(H5Acreate2(loc_id_, name_.c_str(), attr_type_.id(),
                           dataspace().id(), acpl.id(), aapl.id()))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

attribute::attribute(hid_t loc_id_, const std::string& name_)
: attribute_id_(H5Aopen(loc_id_, name_.c_str(), H5P_DEFAULT))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

attribute::~attribute()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Aclose(id()) >= 0,
                           "unable to close the attribute");
    }
}

attribute::attribute(const attribute& other) : attribute_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,
                       "unable to increment the reference count");
}

attribute::attribute(attribute&& other) : attribute_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    other.attribute_id_ = -1;
}

attribute& attribute::operator=(const attribute& other)
{
    using std::swap;

    attribute temp(other);
    swap(*this, temp);

    return *this;
}

attribute& attribute::operator=(attribute&& other)
{
    using std::swap;

    swap(attribute_id_, other.attribute_id_);

    return *this;
}

void attribute::write(const void* value)
{
    if (H5Awrite(id(), H5Aget_type(id()), value) < 0)
        throw_on_hdf5_error();
}

void attribute::read(void* value) const
{
    if (H5Aread(id(), H5Aget_type(id()), value) < 0)
        throw_on_hdf5_error();
}

type attribute::datatype() const
{
    hid_t type_id = H5Aget_type(id());

    if (type_id < 0)
        throw_on_hdf5_error();

    return type(type_id, true);
}

hid_t attribute::id() const
{
    return attribute_id_;
}

bool is_attribute_existing(const object& loc, const std::string& name)
{
    htri_t result = H5Aexists(loc.id(), name.c_str());

    if (result < 0)
        throw_on_hdf5_error();

    return result > 0 ? true : false;
}
}
}
