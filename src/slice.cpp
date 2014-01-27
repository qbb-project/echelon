//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/slice.hpp>

#include <utility>

namespace echelon
{

slice::slice(hdf5::slice native_slice_) : native_slice_(std::move(native_slice_))
{
}

const std::vector<hsize_t>& slice::shape() const
{
    return native_slice_.shape();
}
}
