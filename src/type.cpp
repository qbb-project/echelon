//  Copyright (c) 2012 Christopher Hinz
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
    hdf5::type compound_type = hdf5::type::empty_compound_type(layout.size());

    for (auto& element : layout)
    {
        compound_type.insert_member(element.name, element.offset,
                                    element.type_.native_handle());
    }

    return type(std::move(compound_type));
}

type::type(hdf5::type type_wrapper_) : type_wrapper_(std::move(type_wrapper_))
{
}

type type::clone() const
{
    return type(type_wrapper_.clone());
}

const type::native_handle_type& type::native_handle() const
{
    return type_wrapper_;
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
