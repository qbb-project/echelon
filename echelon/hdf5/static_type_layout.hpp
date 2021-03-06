//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_STATIC_TYPE_LAYOUT_HPP
#define ECHELON_HDF5_STATIC_TYPE_LAYOUT_HPP

namespace echelon
{
namespace hdf5
{
enum class static_type_layout_category
{
    generic,
    packed
};

template <typename T>
struct static_type_layout;

struct id_offset_pair
{
    constexpr id_offset_pair(const char* id, std::size_t offset) : id(id), offset(offset)
    {
    }

    const char* id;
    std::size_t offset;
};

struct id_size_pair
{
    constexpr id_size_pair(const char* id, std::size_t size) : id(id), size(size)
    {
    }

    const char* id;
    std::size_t size;
};
}
}

#endif
