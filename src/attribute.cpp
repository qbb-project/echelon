//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/attribute.hpp>

#include <utility>

namespace echelon
{

attribute::attribute(native_handle_type native_handle_)
: attribute_handle_{std::move(native_handle_)}
{
}

type attribute::datatype() const
{
    return type(attribute_handle_.datatype());
}

attribute::native_handle_type attribute::native_handle() const
{
    return attribute_handle_;
}

attribute::operator bool() const
{
    return static_cast<bool>(native_handle());
}
}
