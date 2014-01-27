//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/slice.hpp>

namespace echelon
{
namespace hdf5
{
slice::slice(hdf5::precursor::dataset sliced_dataset_,
             const std::vector<totally_bound_range_t>& ranges)
: sliced_dataset_(sliced_dataset_), selected_dataspace_(sliced_dataset_.get_space())
{
    std::vector<hsize_t> offset;
    std::vector<hsize_t> count;
    std::vector<hsize_t> stride;

    for (auto r : ranges)
    {
        assert(r.bound() > r.base());

        const hsize_t size = r.bound() - r.base();

        offset.push_back(r.base());
        count.push_back(size / r.stride());
        stride.push_back(r.stride());
    }

    selected_dataspace_.select_hyperslab(H5S_SELECT_SET, offset, stride, count);

    for (hsize_t dim : count)
    {
        if (dim > 1)
        {
            size_.push_back(dim);
        }
    }
}

const std::vector<hsize_t>& slice::shape() const
{
    return size_;
}
}
}
