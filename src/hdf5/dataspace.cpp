//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/error_handling.hpp>

#include <utility>
#include <algorithm>
#include <functional>

namespace echelon
{
namespace hdf5
{

hyperslab_block::hyperslab_block(std::vector<hsize_t> start_,
                                 std::vector<hsize_t> opposite_)
: start_(start_), dims_(opposite_.size())
{
    std::transform(begin(opposite_), end(opposite_), begin(start_),
                   begin(dims_), std::minus<hsize_t>());
}

const std::vector<hsize_t>& hyperslab_block::start() const
{
    return start_;
}

const std::vector<hsize_t>& hyperslab_block::dims() const
{
    return dims_;
}

dataspace::dataspace() : dataspace_id_(H5Screate(H5S_SCALAR))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

dataspace::dataspace(hid_t dataspace_id_) : dataspace_id_(dataspace_id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iget_type(id()) == H5I_DATASPACE,
                       "ID does not refer to a dataspace");
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()), "invalid object ID");
}

dataspace::dataspace(const std::vector<hsize_t>& dims)
: dataspace_id_(H5Screate_simple(dims.size(), dims.data(), 0))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

dataspace::dataspace(const std::vector<hsize_t>& dims,
                     const std::vector<hsize_t>& max_dims)
: dataspace_id_(H5Screate_simple(dims.size(), dims.data(), max_dims.data()))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

dataspace::~dataspace()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Sclose(id()) >= 0,
                           "unable to close the dataspace");
    }
}

dataspace::dataspace(const dataspace& other) : dataspace_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,
                       "unable to increment the reference count");
}

dataspace::dataspace(dataspace&& other) : dataspace_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    other.dataspace_id_ = -1;
}

dataspace& dataspace::operator=(const dataspace& other)
{
    using std::swap;

    dataspace temp(other);
    swap(*this, temp);

    return *this;
}

dataspace& dataspace::operator=(dataspace&& other)
{
    using std::swap;

    swap(dataspace_id_, other.dataspace_id_);

    return *this;
}

hssize_t dataspace::select_npoints() const
{
    auto npoints = H5Sget_select_npoints(id());

    if (npoints < 0)
        throw_on_hdf5_error();

    return npoints;
}

void dataspace::select_elements(H5S_seloper_t op, std::size_t num_elements,
                                const std::vector<hsize_t>& coord)
{
    herr_t error_code =
        H5Sselect_elements(id(), op, num_elements, coord.data());

    if (error_code < 0)
        throw_on_hdf5_error();
}

void dataspace::select_hyperslab(H5S_seloper_t op,
                                 const std::vector<hsize_t>& start,
                                 const std::vector<hsize_t>& stride,
                                 const std::vector<hsize_t>& count,
                                 const std::vector<hsize_t>& block)
{
    herr_t error_code = H5Sselect_hyperslab(
        id(), op, start.data(), stride.data(), count.data(), block.data());

    if (error_code < 0)
        throw_on_hdf5_error();
}

void dataspace::select_hyperslab(H5S_seloper_t op,
                                 const std::vector<hsize_t>& start,
                                 const std::vector<hsize_t>& stride,
                                 const std::vector<hsize_t>& count)
{
    herr_t error_code = H5Sselect_hyperslab(id(), op, start.data(),
                                            stride.data(), count.data(), 0);

    if (error_code < 0)
        throw_on_hdf5_error();
}

void dataspace::select_hyperslab(H5S_seloper_t op,
                                 const std::vector<hsize_t>& start,
                                 const std::vector<hsize_t>& count)
{
    herr_t error_code =
        H5Sselect_hyperslab(id(), op, start.data(), 0, count.data(), 0);

    if (error_code < 0)
        throw_on_hdf5_error();
}

hssize_t dataspace::get_select_hyperslab_nblocks() const
{
    auto nblocks = H5Sget_select_hyper_nblocks(id());

    if (nblocks < 0)
        throw_on_hdf5_error();

    return nblocks;
}

std::vector<hyperslab_block> dataspace::get_select_hyperslab_blocks() const
{
    int rank = H5Sget_simple_extent_ndims(id());

    if (rank < 0)
        throw_on_hdf5_error();

    hssize_t num_of_blocks = get_select_hyperslab_nblocks();

    std::vector<hsize_t> buffer(2 * num_of_blocks * rank);

    herr_t error_code =
        H5Sget_select_hyper_blocklist(id(), 0, num_of_blocks, buffer.data());

    if (error_code < 0)
        throw_on_hdf5_error();

    std::vector<hyperslab_block> blocks;

    for (auto iter = begin(buffer); iter != end(buffer); iter += 2 * rank)
    {
        std::vector<hsize_t> start(iter, iter + rank);
        std::vector<hsize_t> opposite(iter + rank, iter + 2 * rank);

        blocks.push_back(hyperslab_block(start, opposite));
    }

    return blocks;
}

std::vector<hsize_t> dataspace::get_simple_extent_dims() const
{
    auto ndims = get_simple_extent_ndims();

    std::vector<hsize_t> dims(ndims);

    if (H5Sget_simple_extent_dims(id(), dims.data(), 0) < 0)
        throw_on_hdf5_error();

    return dims;
}

std::size_t dataspace::get_simple_extent_ndims() const
{
    auto ndims = H5Sget_simple_extent_ndims(id());

    if (ndims < 0)
        throw_on_hdf5_error();

    return ndims;
}

hid_t dataspace::id() const
{
    return dataspace_id_;
}
}
}
