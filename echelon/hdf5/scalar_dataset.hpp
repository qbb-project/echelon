//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_SCALAR_DATASET_HPP
#define ECHELON_HDF5_SCALAR_DATASET_HPP

#include <echelon/hdf5/object.hpp>

#include <echelon/hdf5/type.hpp>
#include <echelon/hdf5/precursor/dataset.hpp>
#include <echelon/hdf5/precursor/dataspace.hpp>
#include <echelon/hdf5/precursor/property_list.hpp>

#include <echelon/hdf5/object_reference.hpp>
#include <echelon/hdf5/attribute_repository.hpp>

#include <echelon/hdf5/storage_layer.hpp>

namespace echelon
{
namespace hdf5
{
/** \brief A handle to an HDF5 scalar dataset.
 */
class scalar_dataset
{
public:
    /** \brief Type of the underlying HDF5 low-level handle
     */
    using native_handle_type = hdf5::precursor::dataset;

    scalar_dataset(const object& parent, const std::string& name, const type& datatype);

    explicit scalar_dataset(native_handle_type dataset_wrapper_);

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
        write(sink.dataset_wrapper_, value);
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
        read(source.dataset_wrapper_, value);
    }

    /** \brief The value type of the scalar dataset.
     */
    type datatype() const;

    /** \brief A HDF5 object reference to this scalar dataset.
     */
    object_reference ref() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    const native_handle_type& native_handle() const;

private:
    hdf5::precursor::dataset dataset_wrapper_;

public:
    /** \brief The attributes, which are attached to the scalar dataset.
     */
    attribute_repository<scalar_dataset> attributes() const;
};
}
}

#endif
