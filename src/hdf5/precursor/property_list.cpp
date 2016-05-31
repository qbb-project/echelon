//  Copyright (c) 2012-2016 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/precursor/property_list.hpp>
#include <echelon/hdf5/precursor/error_handling.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
property_list_class::property_list_class(hid_t class_id_) : class_id_(class_id_)
{
}

hid_t property_list_class::id() const
{
    return class_id_;
}

property_list::property_list(hid_t property_list_id_) : property_list_id_(property_list_id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || id() == H5P_DEFAULT || H5Iis_valid(id()) > 0,
                       "invalid object ID");
}

property_list::property_list(property_list_class cls_) : property_list_id_(H5Pcreate(cls_.id()))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

property_list::~property_list()
{
    if (id() > -1 && id() != H5P_DEFAULT)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Pclose(id()) >= 0, "unable to close the property list");
    }
}

property_list::property_list(const property_list& other) : property_list_id_(other.id())
{
    if (id() != H5P_DEFAULT)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0, "unable to increment the reference count");
    }
}

property_list::property_list(property_list&& other) : property_list_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0 || id() == H5P_DEFAULT, "invalid object ID");

    other.property_list_id_ = -1;
}

property_list& property_list::operator=(const property_list& other)
{
    using std::swap;

    property_list temp(other);
    swap(*this, temp);

    return *this;
}

property_list& property_list::operator=(property_list&& other)
{
    using std::swap;

    swap(property_list_id_, other.property_list_id_);

    return *this;
}

void property_list::set_chunk(const std::vector<hsize_t>& dims)
{
    herr_t error_code = H5Pset_chunk(id(), dims.size(), dims.data());

    if (error_code < 0)
        throw_on_hdf5_error();
}

void property_list::set_deflate(unsigned int level)
{
    herr_t error_code = H5Pset_deflate(id(), level);

    if (error_code < 0)
        throw_on_hdf5_error();
}

void property_list::set_shuffle()
{
    herr_t error_code = H5Pset_shuffle(id());

    if (error_code < 0)
        throw_on_hdf5_error();
}

std::vector<hsize_t> property_list::get_chunk() const
{
    int chunk_rank = H5Pget_chunk(id(), 0, nullptr);

    if (chunk_rank < 0)
        throw_on_hdf5_error();

    std::vector<hsize_t> chunk_dims(chunk_rank);

    if (H5Pget_chunk(id(), chunk_rank, chunk_dims.data()) < 0)
        throw_on_hdf5_error();

    return chunk_dims;
}

void property_list::set_char_encoding(H5T_cset_t encoding)
{
    if (H5Pset_char_encoding(id(), encoding) < 0)
        throw_on_hdf5_error();
}

H5T_cset_t property_list::get_char_encoding()
{
    H5T_cset_t encoding;

    if (H5Pget_char_encoding(id(), &encoding) < 0)
        throw_on_hdf5_error();

    return encoding;
}

#if !defined(ECHELON_HAVE_1_0_FORMAT_COMPATIBILITY)

void property_list::set_file_space(H5F_file_space_type_t strategy)
{
    set_file_space(strategy, 0);
}

void property_list::set_file_space(H5F_file_space_type_t strategy, hsize_t threshold)
{
    if (H5Pset_file_space(id(), strategy, threshold) < 0)
        throw_on_hdf5_error();
}

#endif

hid_t property_list::id() const
{
    return property_list_id_;
}
}
}
}
