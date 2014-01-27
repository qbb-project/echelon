//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_DATASET_DIMENSIONS_HPP
#define ECHELON_DATASET_DIMENSIONS_HPP

#include <echelon/type.hpp>
#include <echelon/dimension_scale.hpp>

#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/type_factory.hpp>

#include <hdf5.h> // hsize_t

#include <string>
#include <cstddef>

namespace echelon
{

/** \brief Handle to a dataset dimension
 */
class dimension
{
public:
    explicit dimension(hdf5::group containing_group_handle_, std::size_t index_);

    /** \brief Attach a new dimension scale to this dimension.
     *
     *  \param name      Name of the new dimension scale
     *  \param datatype  Datatype of the new dimension scale
     *
     *  \return a handle to the new dimension scale.
     */
    dimension_scale attach_dimension_scale(const std::string& name, const type& datatype);

    /** \brief Attach a new dimension scale to this dimension.
     *
     *  \param name  Name of the new dimension scale
     *
     *  \tparam T    C++ type, which should be used to determine
     *               the dimension scale's value typele
     *
     *  \return a handle to the new dimension scale.
     */
    template <typename T>
    dimension_scale attach_dimension_scale(const std::string& name)
    {
        return attach_dimension_scale(name, type(hdf5::get_hdf5_type<T>()));
    }

    /** \brief The label of the dimension.
     */
    std::string label() const;

    /**  \brief Relabel the dimension.
     *
     *   \param new_label New label of the dimension
     */
    void relabel(const std::string& new_label);

    /** \brief The extend of the dimension.
     */
    hsize_t extend() const;

private:
    hdf5::group containing_group_handle_;
    std::size_t index_;
};

/** \brief Accessor class for the dimensions of a dataset
 */
class dataset_dimensions
{
public:
    explicit dataset_dimensions(hdf5::group containing_group_handle_);

    /** \brief Type of the iterator over all dataset dimensions
     */
    using iterator = std::vector<dimension>::iterator;

    /** \brief Type of the iterator over all dataset dimensions
     */
    using const_iterator = std::vector<dimension>::const_iterator;

    dataset_dimensions(dataset& associated_dataset_, std::size_t rank_);

    /** \brief Access a dimension by index.
     *
     *  \param index Index of the dimension
     *
     *  \return a handle to the specified dimension.
     */
    dimension& operator[](std::size_t index)
    {
        return dimensions_[index];
    }

    /** \brief Access a dimension by index.
     *
     *  \param index Index of the dimension
     *
     *  \return a handle to the specified dimension.
     */
    const dimension& operator[](std::size_t index) const
    {
        return dimensions_[index];
    }

    /** \brief Iterator pointing to the first dimension
     */
    iterator begin()
    {
        return dimensions_.begin();
    }

    /** \brief Iterator pointing just after the last dimension
     */
    iterator end()
    {
        return dimensions_.end();
    }

    /** \brief Iterator pointing to the first dimension
     */
    const_iterator begin() const
    {
        return dimensions_.begin();
    }

    /** \brief Iterator pointing just after the last dimension
     */
    const_iterator end() const
    {
        return dimensions_.end();
    }

private:
    std::vector<dimension> dimensions_;
};
}

#endif
