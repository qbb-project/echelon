//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/dimension_scale.hpp>

#include <utility>

namespace echelon
{

dimension_scale::dimension_scale(hdf5::dimension_scale native_handle_)
: dim_scale_handle_{std::move(native_handle_)}
{
}

std::vector<hsize_t> dimension_scale::shape() const
{
    return dim_scale_handle_.shape();
}

dimension_scale::native_handle_type dimension_scale::native_handle() const
{
    return dim_scale_handle_;
}
}
