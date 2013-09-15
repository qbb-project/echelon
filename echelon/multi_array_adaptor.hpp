//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_MULTI_ARRAY_ADAPTOR_HPP
#define ECHELON_MULTI_ARRAY_ADAPTOR_HPP

#include <echelon/container_adaption.hpp>
#include <echelon/dataset.hpp>
#include <echelon/detail/map_indices.hpp>
#include <echelon/detail/all_integral.hpp>

#include <algorithm>
#include <functional>
#include <vector>

namespace echelon
{

/** \brief An adaptor, around a random-access sequence container, which
 *         behaves like a multidimensional array.
 *
 *  \tparam Container the type of the wrapped container. Container should
 *          provide random access.
 */
template <typename Container>
class multi_array_adaptor
{
public:
    /** \brief value type of the array */
    typedef typename Container::value_type value_type;

    /** \brief Constructs a new adaptor around a given container
     *         using a given shape.

     *  \pre Let \f$ (s_1,...,s_{N}) \f$ be the requested shape and let
     *       \f$ s \f$ be the size of the wrapped container, then \f$ \prod_{i}
     s_{i} = s \f$
     *       should hold.
     *
     *  \param container_ the adapted container
     *  \param shape_ the shape of the array
     */
    multi_array_adaptor(Container& container_, std::vector<std::size_t> shape_)
    : container_(container_), shape_(std::move(shape_))
    {
    }

    /** \brief Accesses a specified element.
     *
     *  \tparam Indices Types of the indices. All indices should have integral
     *type.
     *
     *  \pre Let \f$ \left(s_1,...s_{rank}\right) \f$ be the shape of the array
     *       and let \f$ \left(i_1,...i_{rank_2}\right) \f$ be the tuple of
     *indices,
     *       then \f$ rank = rank_2 \f$ and \f$ i_{i} < s_{i} \f$ for every \f$
     *i \in
     *       \left\{ 0,...,rank-1 \right\} \f$ should hold.
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

        return container_[detail::map_indices(shape_, indices...)];
    }

    /** \brief Accesses a specified element.
     *
     *  \tparam Indices Types of the indices. All indices should have integral
     *type.
     *
     *  \pre Let \f$ \left(s_1,...s_{rank}\right) \f$ be the shape of the array
     *       and let \f$ \left(i_1,...i_{rank_2}\right) \f$ be the tuple of
     *indices,
     *       then \f$ rank = rank_2 \f$ and \f$ i_{i} < s_{i} \f$ for every \f$
     *i \in
     *       \left\{ 0,...,rank-1 \right\} \f$ should hold.
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

        return container_[detail::map_indices(shape_, indices...)];
    }

    /** \brief Returns an iterator, which points to the first element of the
     *flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence,
     *is not specified.
     */
    typename Container::iterator begin()
    {
        return container_.begin();
    }

    /** \brief Returns an iterator, which points to the last element of the
     *flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence,
     *is not specified.
     */
    typename Container::iterator end()
    {
        return container_.end();
    }

    /** \brief Returns an iterator, which points to the first element of the
     *flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence,
     *is not specified.
     */
    typename Container::const_iterator begin() const
    {
        return container_.begin();
    }

    /** \brief Returns an iterator, which points to the last element of the
     *flattened array.
     *
     *  \note The order, in which the elements appear in the flattened sequence,
     *is not specified.
     */
    typename Container::const_iterator end() const
    {
        return container_.end();
    }

    /** \brief Direct access to the underlying array.
     *
     *  \return a pointer to the underlying array.
     */
    const value_type* data() const
    {
        using ::echelon::data;

        return data(container_);
    }

    /** \brief Direct access to the underlying array.
     *
     *  \return a pointer to the underlying array.
     */
    value_type* data()
    {
        using ::echelon::data;

        return data(container_);
    }

    /** \brief The shape of the array.
     */
    const std::vector<std::size_t>& shape() const
    {
        return shape_;
    }

private:
    Container& container_;
    std::vector<std::size_t> shape_;
};
}

#endif
