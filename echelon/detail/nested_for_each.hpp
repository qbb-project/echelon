//  Copyright (c) 2016 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_NESTED_FOR_EACH_HPP
#define ECHELON_NESTED_FOR_EACH_HPP

#include <vector>

namespace echelon
{

template <typename Integer, typename F>
void nested_for_each(const std::vector<Integer>& bounds, F f)
{
    Integer num_dims = bounds.size();

    std::vector<Integer> indices(num_dims);

    for (;;)
    {
        f(indices);

        ++indices.back();

        for (long int i = num_dims; i-- > 1;)
        {
            if (indices[i] >= bounds[i])
            {
                indices[i] = 0;
                ++indices[i - 1];
            }
        }

        if (indices.front() >= bounds.front())
            break;
    }
}
}

#endif