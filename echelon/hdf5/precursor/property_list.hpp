//  Copyright (c) 2012-2016 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_PROPERTY_LIST_HPP
#define ECHELON_HDF5_PRECURSOR_PROPERTY_LIST_HPP

#include <hdf5.h>
#include <vector>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
class property_list_class
{
public:
    explicit property_list_class(hid_t class_id_);

    hid_t id() const;

private:
    hid_t class_id_;
};

class property_list
{
public:
    explicit property_list(hid_t property_list_id_);
    explicit property_list(property_list_class cls_);
    ~property_list();

    property_list(const property_list& other);
    property_list(property_list&& other);

    property_list& operator=(const property_list& other);
    property_list& operator=(property_list&& other);

    void set_chunk(const std::vector<hsize_t>& dims);
    void set_deflate(unsigned int level);
    void set_shuffle();

    void set_char_encoding(H5T_cset_t encoding);
    H5T_cset_t get_char_encoding();

#if !defined(ECHELON_HAVE_1_0_FORMAT_COMPATIBILITY)
    void set_file_space(H5F_file_space_type_t strategy);
    void set_file_space(H5F_file_space_type_t strategy, hsize_t threshold);
#endif

    std::vector<hsize_t> get_chunk() const;

    hid_t id() const;

private:
    hid_t property_list_id_;
};

static const property_list default_property_list = property_list(H5P_DEFAULT);
}
}
}

#endif
