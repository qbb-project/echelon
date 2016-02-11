//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_SLICE_HPP
#define ECHELON_HDF5_SLICE_HPP

#include <echelon/hdf5/precursor/dataspace.hpp>
#include <echelon/hdf5/precursor/dataset.hpp>
#include <echelon/hdf5/storage_layer.hpp>
#include <echelon/hdf5/container_adaption.hpp>
#include <echelon/hdf5/range.hpp>
#include <echelon/hdf5/array_slice.hpp>

#include <cassert>
#include <vector>
#include <tuple>

namespace echelon
{
namespace hdf5
{

/** \brief A slice (rectangular portion) of an HDF5 dataset.
 */
class slice
{
public:
    slice(hdf5::precursor::dataset sliced_dataset_,
          const std::vector<totally_bound_range_t>& ranges);

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
        auto current_shape = shape_adl(source);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = selected_dataspace_;
        hdf5::precursor::type datatype = sliced_dataset_.datatype();

        write(sliced_dataset_, datatype, mem_space, file_space, source);
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
        auto current_shape = shape_adl(sink);
        
        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = source.selected_dataspace_;
        hdf5::precursor::type datatype = source.sliced_dataset_.datatype();

        read(source.sliced_dataset_, datatype, mem_space, file_space, sink);
    }

    /** \brief Writes the content of an array slice into the slice.
     *
     *  The shape of the array must match the shape of the slice.
     *
     *  \tparam T value type of the array slice
     *
     *  \param source the array slice
     */
    template <typename T, typename StorageOrder>
    void operator<<=(const array_slice<T, StorageOrder>& source)
    {
        auto current_shape = source.original_shape();

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = selected_dataspace_;
        hdf5::precursor::type datatype = sliced_dataset_.datatype();

        auto slice_shape = source.shape();
        std::vector<hsize_t> count;

        for (std::size_t i = 0; i < slice_shape.size(); ++i)
        {
            count.push_back(slice_shape[i] / source.stride()[i]);
        }

        mem_space.select_hyperslab(H5S_SELECT_SET, source.offset(), source.stride(), count);

        write(sliced_dataset_, datatype, mem_space, file_space, source);
    }

    /** \brief Reads the content of the slice into an array slice.
     *
     *  \tparam T type of the container; T must satisfy the data sink
     *            requirements.
     *
     *  \param sink the data sink
     *  \param source the slice, which is used as a source
     */
    template <typename T, typename StorageOrder>
    friend void operator<<=(const array_slice<T, StorageOrder>& sink, const slice& source)
    {
        auto current_shape = sink.original_shape();

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = source.selected_dataspace_;
        hdf5::precursor::type datatype = source.sliced_dataset_.datatype();

        auto slice_shape = sink.shape();
        std::vector<hsize_t> count;

        for (std::size_t i = 0; i < slice_shape.size(); ++i)
        {
            count.push_back(slice_shape[i] / sink.stride()[i]);
        }

        mem_space.select_hyperslab(H5S_SELECT_SET, sink.offset(), sink.stride(), count);

        read(source.sliced_dataset_, datatype, mem_space, file_space, sink);
    }

    /** \brief The shape of the slice.
     */
    const std::vector<hsize_t>& shape() const;

private:
    hdf5::precursor::dataset sliced_dataset_;
    hdf5::precursor::dataspace selected_dataspace_;

    std::vector<hsize_t> size_;
};
}
}

#endif
