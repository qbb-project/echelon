//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_SLICE_HPP
#define ECHELON_SLICE_HPP

#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/data_transfer_broker.hpp>

#include <echelon/range.hpp>

#include <cassert>
#include <vector>
#include <tuple>

namespace echelon
{

template<typename T>
inline std::vector<std::size_t> shape(const std::vector<T>& container)
{
    return { container.size() };
}

template<typename C>
inline std::vector<std::size_t> shape(const C& container)
{
    return container.shape();
}

namespace detail
{
    //Function template which simply forwards its arguments to an overload of shape.
    //Its sole purpose is to ensure that the correct overload can be found by ADL.
    template<typename C>
    inline std::vector<std::size_t> shape_adl(const C& container)
    {
        return shape(container);
    }
}

/** \brief A slice (rectangular portion) of an HDF5 dataset.
 */
class slice
{
public:
    slice(hdf5::dataset sliced_dataset_,
          const std::vector<totally_bound_range_t>& ranges);

    /** \brief Writes the content of a data source into the slice.
     *
     *  The shape of the data source must match the shape of the slice.
     *
     *  \tparam T type of the container; T must satisfy the data source requirements.
     *
     *  \param source the data source
     */
    template<typename T>
    void operator<<=(const T& source)
    {
        auto current_shape = detail::shape_adl(source);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::dataspace mem_space(mem_shape);
        hdf5::dataspace file_space = selected_dataspace_;
        hdf5::type datatype = sliced_dataset_.get_type();

        ::echelon::write(sliced_dataset_,datatype,mem_space,file_space,source);
    }

    /** \brief Reads the content of the slice into a data sink.
     *
     *  \tparam T type of the container; T must satisfy the data sink requirements.
     *
     *  \param sink the data sink
     *  \param source the slice, which is used as a source
     */
    template<typename T>
    friend void operator<<=(T& sink,const slice& source)
    {
        std::vector<hsize_t> slice_shape = source.shape();

        hdf5::dataspace mem_space(slice_shape);
        hdf5::dataspace file_space = source.selected_dataspace_;
        hdf5::type datatype = source.sliced_dataset_.get_type();

        ::echelon::read(source.sliced_dataset_,datatype,mem_space,file_space,sink);
    }

    /** \brief The shape of the slice.
     */
    const std::vector<hsize_t>& shape()const;
private:
    hdf5::dataset sliced_dataset_;
    hdf5::dataspace selected_dataspace_;

    std::vector<hsize_t> size_;
};

}

#endif
