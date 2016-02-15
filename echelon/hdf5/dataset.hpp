//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_DATASET_HPP
#define ECHELON_HDF5_DATASET_HPP

#include <echelon/hdf5/object.hpp>

#include <echelon/hdf5/type.hpp>
#include <echelon/hdf5/type_factory.hpp>
#include <echelon/hdf5/object_reference.hpp>

#include <echelon/hdf5/slice.hpp>
#include <echelon/hdf5/range.hpp>
#include <echelon/hdf5/container_adaption.hpp>
#include <echelon/hdf5/array_slice.hpp>

#include <echelon/hdf5/attribute_repository.hpp>
#include <echelon/hdf5/dataset_dimensions.hpp>
#include <echelon/hdf5/storage_layer.hpp>

#include <echelon/hdf5/precursor/dataset.hpp>
#include <echelon/hdf5/precursor/dataspace.hpp>
#include <echelon/hdf5/precursor/property_list.hpp>

#include <echelon/utility/macros.hpp>

#include <vector>
#include <tuple>
#include <string>
#include <type_traits>
#include <cassert>
#include <iterator>
#include <exception>
#include <utility>

namespace echelon
{
namespace hdf5
{

class invalid_layout_exception : public std::exception
{
public:
    explicit invalid_layout_exception(std::string what_) : what_{std::move(what_)}
    {
    }

    const char* what() const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string what_;
};

/** \brief A handle to an HDF5 dataset.
 */
class dataset
{
public:
    /** \brief Type of the underlying HDF5 low-level handle
     */
    using native_handle_type = hdf5::precursor::dataset;

    /** \brief Initializes the handle with its null state.
     */
    dataset() = default;
    
    dataset(const object& parent, const std::string& name, const type& datatype,
            const std::vector<hsize_t>& shape, const std::vector<hsize_t>& max_dims, int comp_level,
            bool auto_chunking, bool shuffle_filter, const std::vector<hsize_t> chunk_shape);

    explicit dataset(native_handle_type native_handle_);

    /** \brief Writes the content of a data source into the dataset.
     *
     *  The shape of the data source must match the shape of the dataset.
     *
     *  \tparam T type of the container; T must satisfy the data source
     *  requirements.
     *
     *  \param sink  the dataset, which is used as a sink
     *  \param source the data source
     */
    template <typename T>
    friend void operator<<=(dataset& sink, const T& source)
    {
        using std::begin;
	    using std::end;

        const auto& current_shape = shape_adl(source);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = sink.dataset_handle_.get_space();
        hdf5::precursor::type datatype = sink.dataset_handle_.datatype();

        write(sink.dataset_handle_, datatype, mem_space, file_space, source);
    }

    /** \brief Reads the content of the dataset into a data sink.
     *
     *  \tparam T type of the container; T must satisfy the data sink
     *  requirements.
     *
     *  \param sink the data sink
     *  \param source the dataset, which is used as a source
     */
    template <typename T>
    friend void operator<<=(T& sink, const dataset& source)
    {
        using std::begin;
	    using std::end;

        const auto& current_shape = shape_adl(sink);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = source.dataset_handle_.get_space();
        hdf5::precursor::type datatype = source.dataset_handle_.datatype();

        read(source.dataset_handle_, datatype, mem_space, file_space, sink);
    }

    /** \brief Writes the content of an array slice into the dataset.
     *
     *  The shape of the slice must match the shape of the dataset.
     *
     *  \tparam T value type of the slice
     *
     *  \param sink  the dataset, which is used as a sink
     *  \param source the array slice
     */
    template <typename T, typename StorageOrder>
    friend void operator<<=(dataset& sink, const array_slice<T, StorageOrder>& source)
    {
        using std::begin;
        using std::end;

        const auto& current_shape = source.original_shape();

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = sink.dataset_handle_.get_space();
        hdf5::precursor::type datatype = sink.dataset_handle_.datatype();

        mem_space.select_hyperslab(H5S_SELECT_SET, source.offset(), source.stride(), source.shape());

        write(sink.dataset_handle_, datatype, mem_space, file_space, source);
    }

    /** \brief Reads the content of the dataset into an array slice.
     *
     *  \tparam T value type of the slice
     *
     *  \param sink the array slice
     *  \param source the dataset, which is used as a source
     */
    template <typename T, typename StorageOrder>
    friend void operator<<=(const array_slice<T, StorageOrder>& sink, const dataset& source)
    {
        using std::begin;
        using std::end;

        const auto& current_shape = sink.original_shape();

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = source.dataset_handle_.get_space();
        hdf5::precursor::type datatype = source.dataset_handle_.datatype();

        mem_space.select_hyperslab(H5S_SELECT_SET, sink.offset(), sink.stride(), sink.shape());

        read(source.dataset_handle_, datatype, mem_space, file_space, sink);
    }
    
    /** \brief Extends the dataset along a given dimension
     * 
     *  \tparam Container type of the container; Container must satisfy the data sink
     *  requirements.
     *  
     *  \param dimension_index index of the extended dimension
     *  \param container       container, which is used to fill the new portions of the dataset
     */
    template <typename Container>
    void extend_along(std::size_t dimension_index, const Container& container) const
    {
        using std::begin;
	    using std::end;
      
        auto extent = shape();
        auto new_extent = extent;
        auto container_shape = shape_adl(container);

        for (std::size_t i = 0; i < extent.size(); ++i)
        {
            if (i != dimension_index && container_shape[i] != extent[i])
                throw invalid_layout_exception(
                    "extent of non-extended dimension differs between container and dataset");
        }

        new_extent[dimension_index] += container_shape[dimension_index];

        dataset_handle_.set_extent(new_extent);

        // FIXME: extend slicing and refactor this code
        std::vector<hsize_t> mem_shape(begin(container_shape), end(container_shape));

        hdf5::precursor::dataspace mem_space(mem_shape);
        hdf5::precursor::dataspace file_space = dataset_handle_.get_space();
        hdf5::precursor::type datatype = dataset_handle_.datatype();

        std::vector<hsize_t> offset(rank());
        offset[dimension_index] = extent[dimension_index];

        file_space.select_hyperslab(H5S_SELECT_SET, offset, mem_shape);

        write(dataset_handle_, datatype, mem_space, file_space, container);
    }

    /** \brief The shape of the dataset.
     */
    std::vector<hsize_t> shape() const;

    /** \brief The rank of the dataset.
     */
    std::size_t rank() const;

    /** \brief The value type of the dataset.
     */
    type datatype() const;

    /** \brief Slices the dataset.
     *
     *  The number of bounds must match the rank of the dataset.
     *
     *  For further information on slicing have a look at the other overload of this function.
     *
     *  \param bounds boundaries of the slice
     */
    slice operator()(const std::vector<totally_bound_range_t>& bounds) const
    {
        return slice(dataset_handle_, bounds);
    }

    /** \brief Slices the dataset.
     *
     *  The boundaries of the slice within the dataset are specified by index
     *  ranges, which can be
     *  constructed using echelon::range.
     *  Currently the following index range specifiers are supported (N and M
     *  are non-negative integral values
     *  and step is a positive integral value):
     *
     *  index range     | semantic
     *  ----------------|-------------------------------------------------------------------------------------------
     *  range(N,M)      | restrict the dimension to the interval \f$ [N,M) \f$.
     *  range(N,M,step) | restrict the dimension to the interval \f$ [N,M) \f$ and only include every step-th value.
     *  N               | restrict the dimension to the single value N. This is equivalent to range(N,N+1)
     *  _               | do not restrict the dimension, but use the full range of indices
     *
     *  Within the range syntax the wildcard _ can be used as the lower or upper
     *  bound to specifiy, that the dimension should not be restricted through this
     *  bound.
     *
     *  The number of index range specifiers must match the rank of the dataset.
     *
     *  If an index range only contains one value, the rank of the slice is
     *  reduced by one.
     *  If none of the index range satisfies this condition, the rank of the
     *  slice equals the rank
     *  of the dataset.
     *
     *  The following example shows, how a slice can be used to access a portion
     *  of a dataset:
     *  \code{.cpp}
     *
     *  auto foo = some_group.create_dataset<double>("foo",{10,10,10});
     *
     *  multi_array<double> data({6,10},1.0);
     *
     *  // set the first dimension to 2 and clamp the second dimension between 2
     *  // and 8. Afterwards the data is written into the 2-slice.
     *  foo(2,range(2,8),_) <<= data;
     *
     *  \endcode
     *
     *  It should be noted, that only the sliced portion of the dataset is
     *  loaded into the main memory.
     *  Therefore slicing is an efficient way to work on large datasets and even
     *  allows us to deal with datasets
     *  , which do not fit into the main memory.
     *
     *  At the time of writing, \ref slice does support most operations, which
     *  are valid on a dataset.
     *
     *  \tparam Args types of the index range specifiers
     *
     *  \param args index range specifiers
     */
    template <typename... Args>
    slice operator()(Args... args) const
    {
        std::vector<hsize_t> current_shape = shape();

        std::vector<totally_bound_range_t> boundaries;

        detail::calculate_slice_boundaries<0, Args...>::eval(current_shape, boundaries, args...);

        return operator()(boundaries);
    }

    /** \brief A HDF5 object reference to this dataset.
     */
    object_reference ref() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    const native_handle_type& native_handle() const;
    
    /** \brief Tests the validity of the handle.
     */
    explicit operator bool() const;
private:
    hdf5::precursor::dataset dataset_handle_;

public:
    /** \brief The attributes, which are attached to the dataset.
     */
    attribute_repository<dataset> attributes() const;

    /** \brief The dimensions of the dataset
     */
    dataset_dimensions dimensions() const;
};
}
}

#endif
