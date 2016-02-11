//  Copyright (c) 2016 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_COLUMN_MAJOR_STORAGE_ORDER_HPP
#define ECHELON_COLUMN_MAJOR_STORAGE_ORDER_HPP

#include <cstddef>

namespace echelon
{
namespace hdf5
{
template<typename Shape>
struct column_major_storage_order
{
public:
    explicit column_major_storage_order(Shape shape_) : shape_(shape_)
    {
    }

    template<typename Indices>
    std::size_t map(const Indices &indices) const
    {
        std::size_t rank = shape_.size();

        assert(rank == indices.size());

        std::size_t address = 0;

        for (std::size_t i = rank; i-- > 0;)
        {
            address = address * shape_[i] + indices[i];
        }

        return address;
    }

private:
    Shape shape_;
};
}

using hdf5::column_major_storage_order;

}

#endif
