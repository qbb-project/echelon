//  Copyright (c) 2013-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_DIMENSION_SCALE_HPP
#define ECHELON_HDF5_DIMENSION_SCALE_HPP

#include <echelon/hdf5/storage_layer.hpp>
#include <echelon/hdf5/container_adaption.hpp> //for shape_adl
#include <echelon/object.hpp>

#include <echelon/hdf5/precursor/dimension_scale.hpp>

#include <string>

namespace echelon
{
namespace hdf5
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
    using native_handle_type = hdf5::precursor::dimension_scale;

    dimension_scale(object location, const std::string& dataset_name, const type& datatype,
                    const std::vector<hsize_t>& extent, const std::string& scale_name);

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
        auto current_shape = shape_adl(source);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = sink.dim_scale_.get_space();
        hdf5::precursor::type datatype = sink.dim_scale_.datatype();

        write(sink.dim_scale_, datatype, mem_space, file_space, source);
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
        auto current_shape = shape_adl(sink);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = source.dim_scale_.get_space();
        hdf5::precursor::type datatype = source.dim_scale_.datatype();

        read(source.dim_scale_, datatype, mem_space, file_space, sink);
    }

    /** \brief The underlying HDF5 low-level handle.
     */
    const native_handle_type& native_handle() const;

private:
    hdf5::precursor::dimension_scale dim_scale_;
};
}
}

#endif
