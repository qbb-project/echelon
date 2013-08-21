//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_MULTI_ARRAY_HPP
#define ECHELON_MULTI_ARRAY_HPP

#include <echelon/container_adaption.hpp>
#include <echelon/detail/map_indices.hpp>
#include <echelon/detail/all_integral.hpp>

#include <vector>
#include <algorithm>
#include <functional>

namespace echelon
{

/** \brief Multidimensional array with runtime rank and shape.
 *
 *  \tparam T value type of the array
 */
template<typename T>
class multi_array
{
public:
    /** \brief value type of the array */
    typedef T value_type;

    /** \brief Creates an empty array.
     */
    multi_array()=default;

    /** \brief Creates an array with a given shape.
     *
     *  \param shape_ the shape of the array
     *  \param value_ value, which is used to initialize every element.
     *                By default this is the default constructed value.
     */
    explicit multi_array(const std::vector<std::size_t>& shape_, const T& value_ = T())
    :data_(std::accumulate(std::begin(shape_),std::end(shape_),
                           std::size_t(1),std::multiplies<std::size_t>()),
           value_),
     shape_(shape_)
    {}

    /** \brief Accesses a specified element.
     *
     *  \tparam Indices Types of the indices. All indices should have integral type.
     *
     *  \pre Let \f$ \left(s_1,...s_{rank}\right) \f$ be the shape of the array
     *       and let \f$ \left(i_1,...i_{rank_2}\right) \f$ be the tuple of indices,
     *       then \f$ rank = rank_2 \f$ and \f$ i_{i} < s_{i} \f$ for every \f$ i \in
     *       \left\{ 0,...,rank-1 \right\} \f$ should hold.
     *
     *  \param indices Indices of the requested element
     *
     *  \return the specified element
     */
    template<typename... Indices>
    const T& operator()(Indices... indices)const
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return data_[detail::map_indices(shape_,indices...)];
    }

    /** \brief Accesses a specified element.
     *
     *  \tparam Indices Types of the indices. All indices should have integral type.
     *
     *  \pre Let \f$ \left(s_1,...s_{rank}\right) \f$ be the shape of the array
     *       and let \f$ \left(i_1,...i_{rank_2}\right) \f$ be the tuple of indices,
     *       then \f$ rank = rank_2 \f$ and \f$ i_{i} < s_{i} \f$ for every \f$ i \in
     *       \left\{ 0,...,rank-1 \right\} \f$ should hold.
     *
     *  \param indices Indices of the requested element
     *
     *  \return the specified element
     */
    template<typename... Indices>
    T& operator()(Indices... indices)
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return data_[detail::map_indices(shape_,indices...)];
    }

    /** \brief Direct access to the underlying array.
     *
     *  \return a pointer to the underlying array.
     */
    const T* data()const
    {
        return data_.data();
    }

    /** \brief Direct access to the underlying array.
     *
     *  \return a pointer to the underlying array.
     */
    T* data()
    {
        return data_.data();
    }

    /** \brief Returns an iterator, which points to the first element of the flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence, is not specified.
     */
    typename std::vector<T>::iterator begin()
    {
        return data_.begin();
    }

    /** \brief Returns an iterator, which points to the last element of the flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence, is not specified.
     */
    typename std::vector<T>::iterator end()
    {
        return data_.end();
    }

    /** \brief Returns an iterator, which points to the first element of the flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence, is not specified.
     */
    typename std::vector<T>::const_iterator begin()const
    {
        return data_.begin();
    }

    /** \brief Returns an iterator, which points to the last element of the flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence, is not specified.
     */
    typename std::vector<T>::const_iterator end()const
    {
        return data_.end();
    }

    /** \brief The shape of the array.
     */
    const std::vector<std::size_t>& shape()const
    {
        return shape_;
    }

    /** \brief Reshapes the array.
     *
     *  \param new_shape new shape of the array
     */
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

}

#endif
