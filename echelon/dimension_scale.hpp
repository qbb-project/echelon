//  Copyright (c) 2013-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_DIMENSION_SCALE_HPP
#define ECHELON_DIMENSION_SCALE_HPP

#include <echelon/hdf5/dimension_scale.hpp>

#include <string>

namespace echelon
{

class dataset;
class type;

/** \brief A handle to a dimension scale.
 */
class dimension_scale
{
public:
    /** \brief Type of the underlying HDF5 low-level handle
     */
    using native_handle_type = hdf5::dimension_scale;

    explicit dimension_scale(hdf5::dimension_scale native_handle_);

    /** \brief The shape of the dimension scale.
     */
    std::vector<hsize_t> shape() const;

    /** \brief Writes the content of a data source into the dimension scale.
     *
     *  The shape of the data source must match the shape of the dimension scale.
     *
     *  \tparam T type of the container; T must satisfy the data source
     *  requirements.
     *
     *  \param sink  the dimension, which is used as a sink
     *  \param source the data source
     */
    template <typename T>
    friend void operator<<=(dimension_scale& sink, const T& source)
    {
        sink.dim_scale_handle_ <<= source;
    }

    /** \brief Reads the content of the dimension scale into a data sink.
     *
     *  \tparam T type of the container; T must satisfy the data sink
     *  requirements.
     *
     *  \param sink the data sink
     *  \param source the dimension scale, which is used as a source
     */
    template <typename T>
    friend void operator<<=(T& sink, const dimension_scale& source)
    {
        sink <<= source.dim_scale_handle_;
    }

    /** \brief The underlying HDF5 low-level handle.
     */
    native_handle_type native_handle() const;

private:
    hdf5::dimension_scale dim_scale_handle_;
};
}

#endif
