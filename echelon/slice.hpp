//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_SLICE_HPP
#define ECHELON_SLICE_HPP

#include <echelon/hdf5/slice.hpp>

#include <echelon/range.hpp>

#include <cassert>
#include <vector>
#include <tuple>

namespace echelon
{

/** \brief A slice (rectangular portion) of an HDF5 dataset.
 */
class slice
{
public:
    explicit slice(hdf5::slice native_slice_);

    /** \brief Writes the content of a data source into the slice.
     *
     *  The shape of the data source must match the shape of the slice.
     *
     *  \tparam T type of the container; T must satisfy the data source
     *            requirements.
     *
     *  \param source the data source
     */
    template <typename T>
    void operator<<=(const T& source)
    {
        native_slice_ <<= source;
    }

    /** \brief Reads the content of the slice into a data sink.
     *
     *  \tparam T type of the container; T must satisfy the data sink
     *            requirements.
     *
     *  \param sink the data sink
     *  \param source the slice, which is used as a source
     */
    template <typename T>
    friend void operator<<=(T& sink, const slice& source)
    {
        sink <<= source.native_slice_;
    }

    /** \brief The shape of the slice.
     */
    const std::vector<hsize_t>& shape() const;

private:
    hdf5::slice native_slice_;
};
}

#endif
