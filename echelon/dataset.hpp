//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_DATASET_HPP
#define ECHELON_DATASET_HPP

#include <echelon/object.hpp>
#include <echelon/type.hpp>
#include <echelon/object_reference.hpp>
#include <echelon/slice.hpp>
#include <echelon/range.hpp>
#include <echelon/attribute_repository.hpp>
#include <echelon/dataset_dimensions.hpp>
#include <echelon/utility/macros.hpp>

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/group.hpp>

#include <vector>
#include <tuple>
#include <string>
#include <type_traits>
#include <cassert>
#include <iterator>

namespace echelon
{

using hdf5::_;

/** \brief A handle to an echelon dataset.
 */
class dataset
{
public:
    using native_handle_type = hdf5::group;

    /** \brief Initializes the handle with its null state.
     */
    dataset() = default;
    
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
        sink.dataset_handle_ <<= source;
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
        sink <<= source.dataset_handle_;
    }
    
    /** \brief Extends the dataset along a given dimension
     * 
     *  \tparam Container type of the container; Container must satisfy the data sink
     *  requirements.
     *  
     *  \param dimension_index index of the extended dimension
     *  \param container       container, which is used to fill the new portions of the dataset
     */
    template<typename Container>
    void extend_along(std::size_t dimension_index, const Container& container)
    {
        dataset_handle_.extend_along(dimension_index, container);
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
        return slice(dataset_handle_(args...));
    }

    /** \brief A echelon object reference to this dataset.
     */
    object_reference ref() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    native_handle_type native_handle() const;

    /** \brief Tests the validity of the handle.
     */
    explicit operator bool() const;
private:
    hdf5::group group_handle_;
    hdf5::dataset dataset_handle_;

public:
    /** \brief The attributes, which are attached to the dataset.
     */
    attribute_repository<dataset> attributes() const;

    /** \brief The dimensions of the dataset
     */
    dataset_dimensions dimensions() const;
};
}

#endif
