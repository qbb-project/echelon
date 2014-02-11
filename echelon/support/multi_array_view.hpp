//  Copyright (c) 2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_MULTI_ARRAY_VIEW_HPP
#define ECHELON_MULTI_ARRAY_VIEW_HPP

#include <echelon/detail/map_indices.hpp>
#include <echelon/detail/all_integral.hpp>

#include <algorithm>
#include <functional>
#include <vector>

namespace echelon
{

/** \brief A multidimensional view onto an array.
 *
 *  \tparam T the value type of the array
 */
template <typename T>
class multi_array_view
{
public:
    /** \brief value type of the array */
    using value_type = T;

    /** \brief Constructs a new view for a given array using a given shape.
     *
     *  \pre Let \f$ (s_1,...,s_{N}) \f$ be the requested shape and let
     *       \f$ s \f$ be the size of the underlying memory block, then \f$ \prod_{i} s_{i} = s \f$
     *       should hold.
     *
     *  \param container_ the adapted container
     *  \param shape_ the shape of the array
     */
    multi_array_view(T* data_, std::vector<std::size_t> shape_)
    : data_(data_), shape_(std::move(shape_))
    {
    }

    /** \brief Accesses a specified element.
     *
     *  \tparam Indices Types of the indices. All indices should have integral
     *                  type.
     *
     *  \pre Let \f$ \left(s_1,...s_{rank}\right) \f$ be the shape of the array
     *       and let \f$ \left(i_1,...i_{rank_2}\right) \f$ be the tuple of indices,
     *       then \f$ rank = rank_2 \f$ and \f$ i_{i} < s_{i} \f$ for every \f$
     *       i \in \left\{ 0,...,rank-1 \right\} \f$ should hold.
     *
     *
     *  \param indices Indices of the requested element
     *
     *  \return the specified element
     */
    template <typename... Indices>
    const value_type& operator()(Indices... indices) const
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return data_[detail::map_indices(shape_, indices...)];
    }

    /** \brief Accesses a specified element.
     *
     *  \tparam Indices Types of the indices. All indices should have integral
     *                  type.
     *
     *  \pre Let \f$ \left(s_1,...s_{rank}\right) \f$ be the shape of the array
     *       and let \f$ \left(i_1,...i_{rank_2}\right) \f$ be the tuple of indices,
     *       then \f$ rank = rank_2 \f$ and \f$ i_{i} < s_{i} \f$ for every \f$
     *        i \in \left\{ 0,...,rank-1 \right\} \f$ should hold.
     *
     *  \param indices Indices of the requested element
     *
     *  \return the specified element
     */
    template <typename... Indices>
    value_type& operator()(Indices... indices)
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return data_[detail::map_indices(shape_, indices...)];
    }

    /** \brief Direct access to the underlying array.
     *
     *  \return a pointer to the underlying array.
     */
    const value_type* data() const
    {
        return data_;
    }

    /** \brief Direct access to the underlying array.
     *
     *  \return a pointer to the underlying array.
     */
    value_type* data()
    {
        return data_;
    }

    /** \brief The shape of the array.
     */
    const std::vector<std::size_t>& shape() const
    {
        return shape_;
    }

private:
    T* data_;
    std::vector<std::size_t> shape_;
};
}

#endif