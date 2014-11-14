//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/type.hpp>

namespace echelon
{

type type::char_()
{
    return type(hdf5::type::char_());
}

type type::short_()
{
    return type(hdf5::type::short_());
}

type type::int_()
{
    return type(hdf5::type::int_());
}

type type::long_()
{
    return type(hdf5::type::long_());
}

type type::long_long()
{
    return type(hdf5::type::long_long());
}

type type::uchar()
{
    return type(hdf5::type::uchar());
}

type type::ushort()
{
    return type(hdf5::type::ushort());
}

type type::uint()
{
    return type(hdf5::type::uint());
}

type type::ulong()
{
    return type(hdf5::type::ulong());
}

type type::ulong_long()
{
    return type(hdf5::type::ulong_long());
}

type type::float_()
{
    return type(hdf5::type::float_());
}

type type::double_()
{
    return type(hdf5::type::double_());
}

type type::string()
{
    return type(hdf5::type::string());
}

type type::object_reference()
{
    return type(hdf5::type::object_reference());
}

type type::compound_type(const type_layout& layout)
{
    hdf5::type_layout hdf5_layout(layout.size());

    for (const auto& element : layout)
    {
        hdf5_layout.add_element(element.name, element.type_.native_handle(), element.offset);
    }

    return type(hdf5::type::compound_type(hdf5_layout));
}

std::size_t type::size() const
{
    return type_handle_.size();
}

type::type(type::native_handle_type native_handle_) : type_handle_(std::move(native_handle_))
{
}

type type::clone() const
{
    return type(type_handle_.clone());
}

const type::native_handle_type& type::native_handle() const
{
    return type_handle_;
}

type::operator bool() const
{
    return static_cast<bool>(native_handle());
}

bool operator==(const type& lhs, const type& rhs)
{
    return lhs.native_handle() == rhs.native_handle();
}

bool operator!=(const type& lhs, const type& rhs)
{
    return lhs.native_handle() != rhs.native_handle();
}
}
