//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/file.hpp>

namespace echelon
{
file::file(const std::string& path, create_mode mode)
: file_wrapper_(path,
                mode == create_mode::truncate ? H5F_ACC_TRUNC : H5F_ACC_EXCL,
                H5P_DEFAULT, H5P_DEFAULT),
  root_group_(*this), attributes(root_group_)
{
}

file::file(const std::string& path, open_mode mode)
: file_wrapper_(path,
                mode == open_mode::read_only ? H5F_ACC_RDONLY : H5F_ACC_RDWR,
                H5P_DEFAULT),
  root_group_(*this), attributes(root_group_)
{
}

file::file(hdf5::file file_wrapper_)
: file_wrapper_(std::move(file_wrapper_)), root_group_(*this),
  attributes(root_group_)
{
}

group file::create_group(const std::string& name)
{
    return root_group_.create_group(name);
}

scalar_dataset file::create_scalar_dataset(const std::string& name,
                                           const type& datatype)
{
    return root_group_.create_scalar_dataset(name, datatype);
}

object file::operator[](const std::string& name) const
{
    return root_group_[name];
}

void file::remove(const std::string& name) const
{
    return root_group_.remove(name);
}

group file::require_group(const std::string& name)
{
    return root_group_.require_group(name);
}

scalar_dataset file::require_scalar_dataset(const std::string& name,
                                            const type& datatype)
{
    return root_group_.require_scalar_dataset(name, datatype);
}

void file::iterate_links(const std::function<void(const link&)>& op) const
{
    root_group_.iterate_links(op);
}

void file::visit_links(const std::function<void(const link&)>& visitor) const
{
    root_group_.visit_links(visitor);
}

void
file::visit_objects(const std::function<void(const object&)>& visitor) const
{
    root_group_.visit_objects(visitor);
}

object_reference file::ref() const
{
    return root_group_.ref();
}

hid_t file::id() const noexcept
{
    return file_wrapper_.id();
}

const hdf5::file& file::get_native_handle() const
{
    return file_wrapper_;
}

group mount(const file& mounted_file, const group& mount_point)
{
    return group(hdf5::mount(mounted_file.get_native_handle(),
                             mount_point.get_native_handle()));
}

group mount(const file& mounted_file, const file& mount_point)
{
    return group(hdf5::mount(mounted_file.get_native_handle(),
                             mount_point.get_native_handle()));
}

void unmount(const group& mount_point)
{
    hdf5::unmount(mount_point.get_native_handle());
}
}
