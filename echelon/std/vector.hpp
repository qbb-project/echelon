//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_STD_VECTOR_HPP
#define ECHELON_STD_VECTOR_HPP

#include <echelon/hdf5/container_adaption.hpp>

#include <vector>
#include <cassert>

namespace echelon
{
namespace hdf5
{ 
    
template <typename T>
inline std::vector<std::size_t> shape(const std::vector<T>& container, adl_enabler)
{
    return {container.size()};
}  
    
template <typename T>
inline void reshape(std::vector<T>& container, const std::vector<std::size_t>& new_shape, adl_enabler)
{
    assert(new_shape.size() == 1);

    container.resize(new_shape[0]);
}

}
}

#endif
