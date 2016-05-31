//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/file.hpp>

#include <utility>

namespace echelon
{
file::file(const std::string& path, create_mode mode)
: file_handle_(path, mode == create_mode::truncate ? hdf5::file::create_mode::truncate
                                                   : hdf5::file::create_mode::exclusive),
  root_group_(*this)
{
    root_group_.attributes().create("echelon.library_version", "0.8.0");
#if !defined(ECHELON_HAVE_1_0_FORMAT_COMPATIBILITY)
    root_group_.attributes().create("echelon.format_version", "2.0.0");
#else
    root_group_.attributes().create("echelon.format_version", "1.0.0");
#endif

}

file::file(const std::string& path, open_mode mode)
: file_handle_(path, mode == open_mode::read_only ? hdf5::file::open_mode::read_only
                                                  : hdf5::file::open_mode::read_write),
  root_group_(*this)
{
}

file::file(file::native_handle_type native_handle_)
: file_handle_(std::move(native_handle_)), root_group_(*this)
{
}

group file::create_group(const std::string& name)
{
    return root_group_.create_group(name);
}

dataset file::create_dataset(const std::string& name, const type& datatype,
                             const std::vector<hsize_t>& dims, const dataset_options& options)
{
    return root_group_.create_dataset(name, datatype, dims, options);
}

dataset file::create_dataset(const std::string& name, const type& datatype,
                             const std::vector<hsize_t>& dims, const std::vector<hsize_t>& max_dims,
                             const dataset_options& options)
{
    return root_group_.create_dataset(name, datatype, dims, max_dims, options);
}

scalar_dataset file::create_scalar_dataset(const std::string& name, const type& datatype)
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

dataset file::require_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims, const dataset_options& options)
{
    return root_group_.require_dataset(name, datatype, dims, options);
}

scalar_dataset file::require_scalar_dataset(const std::string& name, const type& datatype)
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

void file::visit_objects(const std::function<void(const object&)>& visitor) const
{
    root_group_.visit_objects(visitor);
}

object_reference file::ref() const
{
    return root_group_.ref();
}

file::native_handle_type file::native_handle() const
{
    return file_handle_;
}

file::operator bool() const
{
    return static_cast<bool>(native_handle());
}

attribute_repository<group> file::attributes() const
{
    return attribute_repository<group>(root_group_.native_handle());
}

group mount(const file& mounted_file, const group& mount_point)
{
    return group(hdf5::mount(mounted_file.native_handle(), mount_point.native_handle()));
}

group mount(const file& mounted_file, const file& mount_point)
{
    return group(hdf5::mount(mounted_file.native_handle(), mount_point.native_handle()));
}

void unmount(const group& mount_point)
{
    hdf5::unmount(mount_point.native_handle());
}
}
