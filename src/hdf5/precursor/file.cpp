//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/precursor/file.hpp>
#include <echelon/hdf5/precursor/property_list.hpp>
#include <echelon/hdf5/precursor/error_handling.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
file::file(hid_t file_id_) : file_id_(file_id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iget_type(id()) == H5I_FILE, "ID does not refer to a file");
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0, "invalid object ID");
}

file::file(const std::string& name_, unsigned flags_, hid_t fcpl_id_, hid_t fapl_id_)
: file_id_(H5Fcreate(name_.c_str(), flags_, fcpl_id_, fapl_id_))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

file::file(const std::string& name_, unsigned flags_, hid_t fapl_id_)
: file_id_(H5Fopen(name_.c_str(), flags_, fapl_id_))
{
    if (id() < 0)
        throw_on_hdf5_error();
}

file::~file()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

        ECHELON_VERIFY_MSG(H5Fclose(id()) >= 0, "unable to close the file");
    }
}

file::file(const file& other) : file_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0, "unable to increment the reference count");
}

file::file(file&& other) : file_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0, "invalid object ID");

    other.file_id_ = -1;
}

file& file::operator=(const file& other)
{
    using std::swap;

    file temp(other);
    swap(*this, temp);

    return *this;
}

file& file::operator=(file&& other)
{
    using std::swap;

    swap(file_id_, other.file_id_);

    return *this;
}

hid_t file::id() const noexcept
{
    return file_id_;
}

group mount(const file& mounted_file, const group& mount_point)
{
    if (H5Fmount(mount_point.id(), ".", mounted_file.id(), H5P_DEFAULT) < 0)
        throw_on_hdf5_error();

    return mount_point;
}

group mount(const file& mounted_file, const file& mount_point)
{
    if (H5Fmount(mount_point.id(), ".", mounted_file.id(), H5P_DEFAULT) < 0)
        throw_on_hdf5_error();

    return group(mount_point.id(), "/", default_property_list);
}

void unmount(const group& mount_point)
{
    if (H5Funmount(mount_point.id(), ".") < 0)
        throw_on_hdf5_error();
}
}
}
}
