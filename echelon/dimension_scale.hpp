//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_DIMENSION_SCALE_HPP
#define ECHELON_DIMENSION_SCALE_HPP

#include <echelon/hdf5/dimension_scale.hpp>
#include <echelon/data_transfer_broker.hpp>
/*FIXME: dimension_scale should be based directly on dataset,
 *       but we need to eliminate the cyclic dependency first
 */
#include <echelon/slice.hpp> //for shape_adl

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
    
    dimension_scale(const dataset& associated_dataset,
                    const std::string& dataset_name, const type& datatype,
                    const std::vector<hsize_t>& extent,
                    const std::string& scale_name);

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
        auto current_shape = detail::shape_adl(source);

        std::vector<hsize_t> mem_shape(begin(current_shape),
                                       end(current_shape));

        hdf5::dataspace mem_space(mem_shape);
        hdf5::dataspace file_space = sink.dim_scale_.get_space();
        hdf5::type datatype = sink.dim_scale_.datatype();

        ::echelon::write(sink.dim_scale_, datatype, mem_space, file_space,
                         source);
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
        std::vector<hsize_t> file_shape = source.shape();

        hdf5::dataspace mem_space(file_shape);
        hdf5::dataspace file_space = source.dim_scale_.get_space();
        hdf5::type datatype = source.dim_scale_.datatype();

        ::echelon::read(source.dim_scale_, datatype, mem_space, file_space,
                        sink);
    }

    /** \brief The underlying HDF5 low-level handle.
     */
    const native_handle_type& native_handle() const;

private:
    hdf5::dimension_scale dim_scale_;
};
}

#endif
