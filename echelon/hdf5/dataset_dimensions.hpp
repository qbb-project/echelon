//  Copyright (c) 2013-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_DATASET_DIMENSIONS_HPP
#define ECHELON_HDF5_DATASET_DIMENSIONS_HPP

#include <echelon/hdf5/type.hpp>
#include <echelon/hdf5/type_factory.hpp>
#include <echelon/hdf5/object.hpp>
#include <echelon/hdf5/dimension_scale.hpp>

#include <hdf5.h> // hsize_t

#include <vector>
#include <string>
#include <cstddef>

namespace echelon
{
namespace hdf5
{
/** \brief Handle to a dataset dimension
 */
class dimension
{
public:
    dimension(const dataset& associated_dataset_, std::size_t index_);

    /** \brief Attach a new dimension scale to this dimension.
     *
     *  \param scale_name   Name of the new dimension scale
     *  \param location     Location of the new dataset
     *  \param dataset_name Name of the dimension scale's dataset
     *  \param datatype     Datatype of the new dimension scale
     *  \param extent       Extent of the dimension scale
     *
     *  \return a handle to the new dimension scale.
     */
    dimension_scale attach_dimension_scale(const std::string& scale_name, object location,
                                           const std::string& dataset_name, const type& datatype,
                                           const std::vector<hsize_t>& extent);

    /** \brief Attach a new dimension scale to this dimension.
     *
     *  \param scale_name   Name of the new dimension scale
     *  \param location     Location of the new dataset
     *  \param dataset_name Name of the dimension scale's dataset
     *  \param extent       Extent of the dimension scale
     *
     *  \tparam T    C++ type, which should be used to determine
     *               the dimension scale's value typele
     *
     *  \return a handle to the new dimension scale.
     */
    template <typename T>
    dimension_scale attach_dimension_scale(const std::string& scale_name, object location,
                                           const std::string& dataset_name,
                                           const std::vector<hsize_t>& extent)
    {
        return attach_dimension_scale(scale_name, location, dataset_name, get_hdf5_type<T>(),
                                      extent);
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
    const dataset* associated_dataset_;
    std::size_t index_;
};

/** \brief Accessor class for the dimensions of a dataset
 */
class dataset_dimensions
{
public:
    /** \brief Type of the iterator over all dataset dimensions
     */
    typedef std::vector<dimension>::iterator iterator;

    /** \brief Type of the iterator over all dataset dimensions
     */
    typedef std::vector<dimension>::const_iterator const_iterator;

    dataset_dimensions(const dataset& associated_dataset_, std::size_t rank_);

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

    std::size_t count() const;

private:
    std::vector<dimension> dimensions_;
};
}
}

#endif
