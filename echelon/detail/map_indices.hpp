//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_DETAIL_MAP_INDICES_HPP
#define ECHELON_DETAIL_MAP_INDICES_HPP

#include <vector>
#include <cstddef>
#include <algorithm>
#include <initializer_list>
#include <cassert>

#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>

namespace echelon
{
namespace detail
{

template <typename... IndexTypes>
inline std::size_t map_indices(const std::vector<std::size_t>& shape, IndexTypes... indices)
{
    assert(shape.size() == sizeof...(indices));

    const std::initializer_list<std::size_t> indices_ = {static_cast<std::size_t>(indices)...};

    auto first = boost::make_zip_iterator(boost::make_tuple(begin(shape), begin(indices_)));
    auto last = boost::make_zip_iterator(boost::make_tuple(end(shape), end(indices_)));

    return std::accumulate(first, last, static_cast<std::size_t>(0),
                           [](std::size_t acc, const boost::tuple<std::size_t, std::size_t>& value)
                           { return boost::get<0>(value) * acc + boost::get<1>(value); });
}
}
}

#endif
