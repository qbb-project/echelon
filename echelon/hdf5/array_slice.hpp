//  Copyright (c) 2015 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_ARRAY_SLICE_HPP
#define ECHELON_HDF5_ARRAY_SLICE_HPP

#include <echelon/hdf5/range.hpp>
#include <echelon/hdf5/container_adaption.hpp>

#include <vector>
#include <utility>

namespace echelon
{
namespace hdf5
{

template <typename T>
class array_slice
{
public:
    array_slice(T* data_, std::vector<hsize_t> original_shape_, std::vector<hsize_t> offset_,
                std::vector<hsize_t> shape_, std::vector<hsize_t> stride_)
    : data_(std::move(data_)), original_shape_(std::move(original_shape_)),
      offset_(std::move(offset_)), shape_(std::move(shape_)), stride_(std::move(stride_))
    {
    }

    template <typename... Indices>
    T operator()(Indices... indices) const
    {
        std::vector<hsize_t> indices_({static_cast<hsize_t>(indices)...});

        hsize_t linear_index = 0;

        for (std::size_t i = indices_.size(); i-- > 0;)
        {
            linear_index =
                linear_index * original_shape_[i] + (offset_[i] + indices_[i] * stride_[i]);
        }

        return data_[linear_index];
    }

    T* data() const
    {
        return data_;
    }

    const std::vector<hsize_t>& original_shape() const
    {
        return original_shape_;
    }

    const std::vector<hsize_t>& offset() const
    {
        return offset_;
    }

    const std::vector<hsize_t>& shape() const
    {
        return shape_;
    }

    const std::vector<hsize_t>& stride() const
    {
        return stride_;
    }

private:
    T* data_;
    std::vector<hsize_t> original_shape_;
    std::vector<hsize_t> offset_;
    std::vector<hsize_t> shape_;
    std::vector<hsize_t> stride_;
};

/** \brief Slice a container.
 *
 *  \tparam C type of the container; must fulfill the Container requirements
 *  \tparam Args types of the index range specifiers
 *
 *  \param container the sliced container
 *  \param args index range specifiers
 */
template <typename C, typename... Args>
array_slice<typename container_trait<C>::value_type> make_slice(C&& container, Args... args)
{
    static_assert(is_container<C>(), "C does not fulfill the Container requirements.");

    using value_type = typename container_trait<C>::value_type;

    std::vector<hsize_t> shape_;

    auto shape = echelon::hdf5::shape_adl(container);

    for (auto extent : shape)
    {
        shape_.push_back(extent);
    }

    std::vector<totally_bound_range_t> slice_boundaries;

    detail::calculate_slice_boundaries<0, Args...>::eval(shape_, slice_boundaries, args...);

    std::vector<hsize_t> offset;
    std::vector<hsize_t> slice_shape;
    std::vector<hsize_t> stride;

    for (auto bounds : slice_boundaries)
    {
        auto base = bounds.base();

        offset.push_back(base);
        slice_shape.push_back(bounds.bound() - base);
        stride.push_back(bounds.stride());
    }

    return array_slice<value_type>(echelon::hdf5::data_adl(container), std::move(shape_),
                                   std::move(offset), std::move(slice_shape), std::move(stride));
}
}
}

#endif
