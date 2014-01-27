//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_CONTAINER_ADAPTION_HPP
#define ECHELON_HDF5_CONTAINER_ADAPTION_HPP

#include <echelon/std/stl_support.hpp>

namespace echelon
{
namespace hdf5
{
template <typename C>
inline auto data(const C& container) -> decltype(container.data())
{
    return container.data();
}

template <typename C>
inline auto data(C& container) -> decltype(container.data())
{
    return container.data();
}

/** \brief Reshapes the container.
 *
 *  \tparam C type of the container
 *
 *  \param container container, which should be reshaped
 *  \param new_shape new shape of the container
 */
template <typename C>
inline void reshape(C& container, const std::vector<std::size_t>& new_shape)
{
    container.reshape(new_shape);
}
}
}

#endif
