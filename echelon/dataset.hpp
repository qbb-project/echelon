#ifndef ECHELON_DATASET_HPP
#define ECHELON_DATASET_HPP

#include <echelon/object.hpp>

#include <echelon/type.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/property_list.hpp>
#include <echelon/type_factory.hpp>
#include <echelon/object_reference.hpp>

#include <echelon/slice.hpp>
#include <echelon/range.hpp>

#include <echelon/attribute_repository.hpp>
#include <echelon/dataset_dimensions.hpp>
#include <echelon/data_transfer_broker.hpp>

#include <vector>
//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <string>
#include <type_traits>
#include <cassert>

namespace echelon
{

struct unbound_t
{
};

static const unbound_t _ = {};

namespace detail
{

template <std::size_t I, typename... Args>
struct calculate_slice_boundaries;

template <std::size_t I>
struct calculate_slice_boundaries<I>
{
    static void eval(const std::vector<hsize_t>&,
                     std::vector<totally_bound_range_t>&)
    {
    }
};

template <std::size_t I, typename Front, typename... Tail>
struct calculate_slice_boundaries<I, Front, Tail...>
{
    static void eval(const std::vector<hsize_t>& current_shape,
                     std::vector<totally_bound_range_t>& boundaries,
                     Front front, Tail... tail)
    {
        boundaries.push_back(get_boundaries(current_shape[I], front));

        calculate_slice_boundaries<I + 1, Tail...>::eval(current_shape,
                                                         boundaries, tail...);
    }

private:
    static totally_bound_range_t get_boundaries(hsize_t extend, unbound_t)
    {
        return range(0, extend);
    }

    template <typename Base>
    static totally_bound_range_t get_boundaries(hsize_t extend,
                                                range_t<Base, unbound_t> r)
    {
        return range(r.base(), extend);
    }

    template <typename Bound>
    static totally_bound_range_t get_boundaries(hsize_t,
                                                range_t<unbound_t, Bound> r)
    {
        return range(0, r.bound());
    }

    template <typename Base, typename Bound>
    static totally_bound_range_t get_boundaries(hsize_t, range_t<Base, Bound> r)
    {
        static_assert(
            std::is_integral<Base>::value && std::is_integral<Bound>::value,
            "only integral values are allowed in slicing expressions");

        return r;
    }

    template <typename T>
    static totally_bound_range_t get_boundaries(hsize_t extend, T value)
    {
        static_assert(
            std::is_integral<T>::value,
            "only integral values are allowed in slicing expressions");

        assert(value < extend);

        return range(value, value + 1);
    }
};
}

/** \brief A handle to an HDF5 dataset.
 */
class dataset
{
public:
    dataset(const object& parent, const std::string& name, const type& datatype,
            const std::vector<hsize_t>& shape, int comp_level,
            const std::vector<hsize_t> chunk_shape);

    explicit dataset(hdf5::dataset dataset_wrapper_);

    /** \brief Writes the content of a data source into the dataset.
     *
     *  The shape of the data source must match the shape of the dataset.
     *
     *  \tparam T type of the container; T must satisfy the data source
     *requirements.
     *
     *  \param sink  the dataset, which is used as a sink
     *  \param source the data source
     */
    template <typename T>
    friend void operator<<=(dataset& sink, const T& source)
    {
        auto current_shape = detail::shape_adl(source);

        std::vector<hsize_t> mem_shape(begin(current_shape),
                                       end(current_shape));

        hdf5::dataspace mem_space(mem_shape);
        hdf5::dataspace file_space = sink.dataset_wrapper_.get_space();
        hdf5::type datatype = sink.dataset_wrapper_.datatype();

        ::echelon::write(sink.dataset_wrapper_, datatype, mem_space, file_space,
                         source);
    }

    /** \brief Reads the content of the dataset into a data sink.
     *
     *  \tparam T type of the container; T must satisfy the data sink
     *requirements.
     *
     *  \param sink the data sink
     *  \param source the dataset, which is used as a source
     */
    template <typename T>
    friend void operator<<=(T& sink, const dataset& source)
    {
        std::vector<hsize_t> file_shape = source.shape();

        hdf5::dataspace mem_space(file_shape);
        hdf5::dataspace file_space = source.dataset_wrapper_.get_space();
        hdf5::type datatype = source.dataset_wrapper_.datatype();

        ::echelon::read(source.dataset_wrapper_, datatype, mem_space,
                        file_space, sink);
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
     *ranges, which can be
     *  constructed using echelon::range.
     *  Currently the following index range specifiers are supported (N and M
     *are non-negative integral values
     *  and step is a positive integral value):
     *
     *  index range     | semantic
     *  ----------------|-------------------------------------------------------------------------------------------
     *  range(N,M)      | restrict the dimension to the interval \f$ [N,M) \f$.
     *  range(N,M,step) | restrict the dimension to the interval \f$ [N,M) \f$
     *and only include every step-th value.
     *  N               | restrict the dimension to the single value N. This is
     *equivalent to range(N,N+1)
     *  _               | do not restrict the dimension, but use the full range
     *of indices
     *
     *  Within the range syntax the wildcard _ can be used as the lower or upper
     *bound to
     *  specifiy, that the dimension should not be restricted through this
     *bound.
     *
     *  The number of index range specifiers must match the rank of the dataset.
     *
     *  If an index range only contains one value, the rank of the slice is
     *reduced by one.
     *  If none of the index range satisfies this condition, the rank of the
     *slice equals the rank
     *  of the dataset.
     *
     *  The following example shows, how a slice can be used to access a portion
     *of a dataset:
     *  \code{.cpp}
     *
     *  auto foo = some_group.create_dataset<double>("foo",{10,10,10});
     *
     *  multi_array<double> data({6,10},1.0);
     *
     *  // set the first dimension to 2 and clamp the second dimension between 2
     *and 8.
     *  // Afterwards the data is written into the 2-slice.
     *  foo.slice(2,range(2,8),_) <<= data;
     *
     *  \endcode
     *
     *  It should be noted, that only the sliced portion of the dataset is
     *loaded into the main memory.
     *  Therefore slicing is an efficient way to work on large datasets and even
     *allows us to deal with datasets
     *  , which do not fit into the main memory.
     *
     *  At the time of writing, \ref slice does support most operations, which
     *are valid on a dataset.
     *
     *  \tparam Args types of the index range specifiers
     *
     *  \param args index range specifiers
     */
    template <typename... Args>
    slice slice(Args... args) const
    {
        std::vector<hsize_t> current_shape = shape();

        std::vector<totally_bound_range_t> boundaries;

        detail::calculate_slice_boundaries<0, Args...>::eval(
            current_shape, boundaries, args...);

        return ::echelon::slice(dataset_wrapper_, boundaries);
    }

    /** \brief A HDF5 object reference to this dataset.
     */
    object_reference ref() const;

    /** \brief The ID, which corresponds to the underlying HDF5 object.
     */
    hid_t id() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    const hdf5::dataset& get_native_handle() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    hdf5::dataset& get_native_handle();

private:
    hdf5::dataset dataset_wrapper_;

public:
    /** \brief The attributes, which are attached to the dataset.
     */
    attribute_repository<dataset> attributes;

    /** \brief The dimensions of the dataset
     */
    dataset_dimensions dimensions;
};
}

#endif
