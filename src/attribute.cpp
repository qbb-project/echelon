//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/attribute.hpp>

namespace echelon
{

attribute::attribute(const object& parent, const std::string& name,
                     const type& datatype)
:attribute_wrapper_(parent.id(),name,datatype.get_native_type())
{}

attribute::attribute(const object& parent, const std::string& name)
:attribute_wrapper_(parent.id(),name)
{}

type attribute::datatype()const
{
    return type(attribute_wrapper_.datatype());
}

hid_t attribute::id()const
{
    return attribute_wrapper_.id();
}

const hdf5::attribute& attribute::get_native_handle()const
{
    return attribute_wrapper_;
}

}
