//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_SCALAR_DATASET_HPP
#define ECHELON_SCALAR_DATASET_HPP

#include <echelon/object.hpp>

#include <echelon/type.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/scalar_dataset.hpp>

#include <echelon/object_reference.hpp>
#include <echelon/attribute_repository.hpp>

namespace echelon
{

/** \brief A handle to an HDF5 scalar dataset.
 */
class scalar_dataset
{
public:
    /** \brief Type of the underlying HDF5 low-level handle
     */
    using native_handle_type = hdf5::group;

    explicit scalar_dataset(native_handle_type native_handle_);

    /** \brief Writes the content of a variable into the scalar dataset.
     *
     *  \tparam T Type of the written value.
     *
     *  \param sink  the scalar dataset, which is used as a sink
     *  \param value value, which is written into the dataset
     */
    template <typename T>
    friend inline void operator<<=(scalar_dataset& sink, const T& value)
    {
        sink.scalar_dataset_handle_ <<= value;
    }

    /** \brief Reads the content of the scalar dataset into a variable.
     *
     *  \tparam T Type of the read value.
     *
     *  \param value value, which is read from the dataset
     *  \param source  the scalar dataset, which is used as a source
     */
    template <typename T>
    friend inline void operator<<=(T& value, const scalar_dataset& source)
    {
        value <<= source.scalar_dataset_handle_;
    }

    /** \brief The value type of the scalar dataset.
     */
    type datatype() const;

    /** \brief A HDF5 object reference to this scalar dataset.
     */
    object_reference ref() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    native_handle_type native_handle() const;

private:
    hdf5::group group_handle_;
    hdf5::scalar_dataset scalar_dataset_handle_;

public:
    /** \brief The attributes, which are attached to the scalar dataset.
     */
    attribute_repository<scalar_dataset> attributes() const;
};
}

#endif
