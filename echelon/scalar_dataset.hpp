//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 
#ifndef ECHELON_SCALAR_DATASET_HPP
#define ECHELON_SCALAR_DATASET_HPP

#include <echelon/object.hpp>

#include <echelon/type.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/property_list.hpp>

#include <echelon/object_reference.hpp>
#include <echelon/attribute_repository.hpp>

#include <echelon/data_transfer_broker.hpp>

namespace echelon
{

/** \brief A handle to an HDF5 scalar dataset.
 */
class scalar_dataset
{
public:
    scalar_dataset(const object& parent, const std::string& name,
                   const type& datatype);

    explicit scalar_dataset(hdf5::dataset dataset_wrapper_);

    /** \brief Writes the content of a variable into the scalar dataset.
     *
     *  The shape of the data source must match the shape of the dataset.
     *
     *  \tparam T Type of the written value.
     *
     *  \param sink  the scalar dataset, which is used as a sink
     *  \param value value, which is written into the dataset
     */
    template<typename T>
    friend inline void operator<<=(scalar_dataset& sink,const T& value)
    {
        write(sink.dataset_wrapper_,value);
    }

    /** \brief Reads the content of the scalar dataset into a variable.
     *
     *  The shape of the data source must match the shape of the dataset.
     *
     *  \tparam T Type of the read value.
     *
     *  \param value value, which is read from the dataset
     *  \param source  the scalar dataset, which is used as a source
     */
    template<typename T>
    friend inline void operator<<=(T& value,const scalar_dataset& source)
    {
        read(source.dataset_wrapper_,value);
    }

    /** \brief The value type of the scalar dataset.
     */
    type datatype()const;

    /** \brief A HDF5 object reference to this scalar dataset.
     */
    object_reference ref()const;

    /** \brief The ID, which corresponds to the underlying HDF5 object.
     */
    hid_t id()const;

    /** \brief The underlying HDF5 low-level handle.
     */
    const hdf5::dataset& get_native_handle()const;
private:
    hdf5::dataset dataset_wrapper_;
public:
    /** \brief The attributes, which are attached to the scalar dataset.
     */
    attribute_repository<scalar_dataset> attributes;
};

}

#endif
