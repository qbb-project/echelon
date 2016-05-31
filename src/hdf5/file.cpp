//  Copyright (c) 2012-2016 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/file.hpp>

#include <echelon/hdf5/precursor/property_list.hpp>

namespace echelon
{
namespace hdf5
{

namespace
{
precursor::property_list make_default_creation_property_list()
{
    precursor::property_list default_creation_property_list(precursor::property_list_class(H5P_FILE_CREATE));

#if !defined(ECHELON_HAVE_1_0_FORMAT_COMPATIBILITY)
    default_creation_property_list.set_file_space(H5F_FILE_SPACE_ALL_PERSIST);
#endif

    return default_creation_property_list;
}
}

file::file(const std::string& path, create_mode mode)
: file_wrapper_(path, mode == create_mode::truncate ? H5F_ACC_TRUNC : H5F_ACC_EXCL,
                make_default_creation_property_list().id(), H5P_DEFAULT),
  root_group_(*this)
{
}

file::file(const std::string& path, open_mode mode)
: file_wrapper_(path, mode == open_mode::read_only ? H5F_ACC_RDONLY : H5F_ACC_RDWR, H5P_DEFAULT),
  root_group_(*this)
{
}

file::file(hdf5::precursor::file file_wrapper_)
: file_wrapper_(std::move(file_wrapper_)), root_group_(*this)
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

const file::native_handle_type& file::native_handle() const
{
    return file_wrapper_;
}

file::operator bool() const
{
    return static_cast<bool>(native_handle());
}

attribute_repository<group> file::attributes() const
{
    return attribute_repository<group>(root_group_);
}

group mount(const file& mounted_file, const group& mount_point)
{
    return group(hdf5::precursor::mount(mounted_file.native_handle(), mount_point.native_handle()));
}

group mount(const file& mounted_file, const file& mount_point)
{
    return group(hdf5::precursor::mount(mounted_file.native_handle(), mount_point.native_handle()));
}

void unmount(const group& mount_point)
{
    hdf5::precursor::unmount(mount_point.native_handle());
}
}
}
