//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_DATASPACE_HPP
#define ECHELON_HDF5_PRECURSOR_DATASPACE_HPP

#include <hdf5.h>
#include <vector>

namespace echelon
{
namespace hdf5
{
namespace precursor
{

constexpr auto unlimited = H5S_UNLIMITED;

class hyperslab_block
{
public:
    hyperslab_block(std::vector<hsize_t> start_, std::vector<hsize_t> opposite_);

    const std::vector<hsize_t>& start() const;
    const std::vector<hsize_t>& dims() const;

private:
    std::vector<hsize_t> start_;
    std::vector<hsize_t> dims_;
};

class dataspace
{
public:
    dataspace();
    explicit dataspace(hid_t dataspace_id_);
    explicit dataspace(const std::vector<hsize_t>& dims);
    dataspace(const std::vector<hsize_t>& dims, const std::vector<hsize_t>& max_dims);
    ~dataspace();

    dataspace(const dataspace& other);
    dataspace(dataspace&& other);

    dataspace& operator=(const dataspace& other);
    dataspace& operator=(dataspace&& other);

    hssize_t select_npoints() const;

    void select_elements(H5S_seloper_t op, std::size_t num_elements,
                         const std::vector<hsize_t>& coord);
    void select_hyperslab(H5S_seloper_t op, const std::vector<hsize_t>& start,
                          const std::vector<hsize_t>& stride, const std::vector<hsize_t>& count,
                          const std::vector<hsize_t>& block);

    void select_hyperslab(H5S_seloper_t op, const std::vector<hsize_t>& start,
                          const std::vector<hsize_t>& stride, const std::vector<hsize_t>& count);

    void select_hyperslab(H5S_seloper_t op, const std::vector<hsize_t>& start,
                          const std::vector<hsize_t>& count);

    hssize_t get_select_hyperslab_nblocks() const;
    std::vector<hyperslab_block> get_select_hyperslab_blocks() const;

    std::vector<hsize_t> get_simple_extent_dims() const;
    std::size_t get_simple_extent_ndims() const;

    hid_t id() const;

private:
    hid_t dataspace_id_;
};
}
}
}

#endif
