//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/attribute.hpp>

namespace echelon
{
namespace hdf5
{
attribute::attribute(const object& parent, const std::string& name, const type& datatype)
: attribute_wrapper_(-1)
{
    hdf5::precursor::property_list attribute_creation_properties(
        hdf5::precursor::property_list_class(H5P_ATTRIBUTE_CREATE));
    attribute_creation_properties.set_char_encoding(H5T_CSET_UTF8);

    attribute_wrapper_ = hdf5::precursor::attribute(
        parent.native_handle().id(), name, datatype.native_handle(), attribute_creation_properties,
        hdf5::precursor::default_property_list);
}

attribute::attribute(const object& parent, const std::string& name)
: attribute_wrapper_(parent.native_handle().id(), name)
{
}

type attribute::datatype() const
{
    return type(attribute_wrapper_.datatype());
}

const attribute::native_handle_type& attribute::native_handle() const
{
    return attribute_wrapper_;
}
}
}
