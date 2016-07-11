//  Copyright (c) 2012-2016 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_DETAIL_MAP_INDICES_HPP
#define ECHELON_DETAIL_MAP_INDICES_HPP

#include <echelon/hdf5/container_adaption.hpp>

#include <vector>
#include <array>
#include <cstddef>
#include <cassert>

namespace echelon
{
namespace detail
{

template <typename... IndexTypes>
inline std::size_t map_indices(const std::vector<std::size_t>& shape, IndexTypes... indices)
{
    constexpr std::size_t number_of_incides = sizeof...(indices);

    assert(shape.size() == number_of_incides);

    std::array<std::size_t, number_of_incides> index_pack = {static_cast<std::size_t>(indices)...};

    return hdf5::row_major_storage_order<std::vector<std::size_t>>(shape).map(index_pack);
}
}
}

#endif
