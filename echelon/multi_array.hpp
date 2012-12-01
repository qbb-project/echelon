//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_MULTI_ARRAY_HPP
#define ECHELON_MULTI_ARRAY_HPP

#include <echelon/customization_hooks.hpp>
#include <echelon/detail/map_indices.hpp>
#include <echelon/detail/all_integral.hpp>

#include <vector>
#include <algorithm>
#include <functional>

namespace echelon
{

template<typename T>
class multi_array
{
public:
    typedef T value_type;

    multi_array()=default;

    explicit multi_array(const std::vector<std::size_t>& shape_)
    :data_(std::accumulate(std::begin(shape_),std::end(shape_),
                           std::size_t(1),std::multiplies<std::size_t>())),
     shape_(shape_)
    {}

    template<typename... Indices>
    const T& operator()(Indices... indices)const
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return data_[detail::map_indices(shape_,indices...)];
    }

    template<typename... Indices>
    T& operator()(Indices... indices)
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return data_[detail::map_indices(shape_,indices...)];
    }

    const T* data()const
    {
        return data_.data();
    }

    T* data()
    {
        return data_.data();
    }

    typename std::vector<T>::iterator begin()
    {
        return data_.begin();
    }

    typename std::vector<T>::iterator end()
    {
        return data_.end();
    }

    typename std::vector<T>::const_iterator begin()const
    {
        return data_.begin();
    }

    typename std::vector<T>::const_iterator end()const
    {
        return data_.end();
    }

    const std::vector<std::size_t>& shape()const
    {
        return shape_;
    }

    void reshape(const std::vector<std::size_t>& new_shape)
    {
        data_.resize(std::accumulate(std::begin(new_shape),std::end(new_shape),
                                     std::size_t(1),std::multiplies<std::size_t>()));

        this->shape_ = new_shape;
    }
private:
    std::vector<T> data_;
    std::vector<std::size_t> shape_;
};

template<typename T>
inline void reshape(multi_array<T>& container,
                    const std::vector<std::size_t>& new_shape)
{
    container.reshape(new_shape);
}

}

#endif
