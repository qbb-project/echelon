//  Copyright (c) 2015 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_ARRAY_SLICE_HPP
#define ECHELON_HDF5_ARRAY_SLICE_HPP

#include <echelon/hdf5/range.hpp>
#include <echelon/hdf5/container_adaption.hpp>

#include <vector>
#include <array>
#include <utility>

namespace echelon
{
namespace hdf5
{

template <typename StorageOrder>
class array_slice_storage_order
{
public:
    array_slice_storage_order(const std::vector<hsize_t>& offset_,
                              const std::vector<hsize_t>& stride_,
                              const StorageOrder& underlying_storage_order_)
    : offset_(&offset_), stride_(&stride_), underlying_storage_order_(&underlying_storage_order_)
    {
    }

    template <typename Indices>
    hsize_t map(const Indices& indices) const
    {
        auto num_dims = indices.size();

        std::vector<hsize_t> modified_indices;
        modified_indices.reserve(num_dims);

        for (decltype(num_dims) i = 0; i < num_dims; ++i)
        {
            modified_indices.push_back((*offset_)[i] + indices[i] * (*stride_)[i]);
        }

        return underlying_storage_order_->map(modified_indices);
    }

private:
    const std::vector<hsize_t>* offset_;
    const std::vector<hsize_t>* stride_;
    const StorageOrder* underlying_storage_order_;
};

template <typename StorageOrder>
struct is_native_storage_order<array_slice_storage_order<StorageOrder>>
    : is_native_storage_order<StorageOrder>
{
};

template <typename T, typename StorageOrder>
class array_slice
{
public:
    array_slice(T* data_, std::vector<hsize_t> original_shape_,
                StorageOrder underlying_storage_order_, std::vector<hsize_t> offset_,
                std::vector<hsize_t> shape_, std::vector<hsize_t> stride_)
    : data_(std::move(data_)), original_shape_(std::move(original_shape_)),
      underlying_storage_order_(std::move(underlying_storage_order_)), offset_(std::move(offset_)),
      shape_(std::move(shape_)), stride_(std::move(stride_))
    {
    }

    template <typename... Indices>
    const T& operator()(Indices... indices) const
    {
        std::array<hsize_t, sizeof...(indices)> indices_({static_cast<hsize_t>(indices)...});

        auto order = storage_order(*this);

        return data_[order.map(indices_)];
    }

    template <typename... Indices>
    T& operator()(Indices... indices)
    {
        std::array<hsize_t, sizeof...(indices)> indices_({static_cast<hsize_t>(indices)...});

        auto order = storage_order(*this);

        return data_[order.map(indices_)];
    }

    T* data() const
    {
        return data_;
    }

    const std::vector<hsize_t>& original_shape() const
    {
        return original_shape_;
    }

    const StorageOrder& underlying_storage_order() const
    {
        return underlying_storage_order_;
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
    StorageOrder underlying_storage_order_;
    std::vector<hsize_t> offset_;
    std::vector<hsize_t> shape_;
    std::vector<hsize_t> stride_;
};

template <typename T, typename StorageOrder>
inline array_slice_storage_order<StorageOrder>
storage_order(const array_slice<T, StorageOrder>& container, adl_enabler)
{
    return array_slice_storage_order<StorageOrder>(container.offset(), container.stride(),
                                                   container.underlying_storage_order());
}

/** \brief Slice a container.
 *
 *  \tparam C type of the container; must fulfill the Container requirements
 *
 *  \param container the sliced container
 *  \param slice_boundaries boundaries of the slice
 */
template <typename C>
auto make_slice(C&& container, const std::vector<totally_bound_range_t>& slice_boundaries)
-> array_slice<typename container_trait<C>::value_type, decltype(storage_order_adl(container))>
{
    static_assert(is_container<C>(), "C does not fulfill the Container requirements.");

    using value_type = typename container_trait<C>::value_type;

    std::vector<hsize_t> shape_;

    auto shape = echelon::hdf5::shape_adl(container);

    for (auto extent : shape)
    {
        shape_.push_back(extent);
    }

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

    auto num_dims = slice_shape.size();

    for (decltype(num_dims) i = 0; i < num_dims; ++i)
    {
        slice_shape[i] /= stride[i];
    }

    return array_slice<value_type, decltype(storage_order_adl(container))>(
            echelon::hdf5::data_adl(container), std::move(shape_), storage_order_adl(container),
            std::move(offset), std::move(slice_shape), std::move(stride));
}

/** \brief Slice a container.
 *
 *  \tparam C type of the container; must fulfill the Container requirements
 *  \tparam Args types of the index range specifiers
 *
 *  \param container the sliced container
 *  \param args index range specifiers
 */
template <typename C, typename... Args>
auto make_slice(C&& container, Args... args)
    -> array_slice<typename container_trait<C>::value_type, decltype(storage_order_adl(container))>
{
    static_assert(is_container<C>(), "C does not fulfill the Container requirements.");

    std::vector<hsize_t> shape_;

    auto shape = echelon::hdf5::shape_adl(container);

    for (auto extent : shape)
    {
        shape_.push_back(extent);
    }

    std::vector<totally_bound_range_t> slice_boundaries;

    detail::calculate_slice_boundaries<0, Args...>::eval(shape_, slice_boundaries, args...);

    return make_slice(container, slice_boundaries);
}
}
}

#endif
