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
                H5P_DEFAULT,
                H5P_DEFAULT),
  root_group_(*this),
  attributes(root_group_)
{
}

file::file(const std::string& path, open_mode mode)
: file_wrapper_(path,
                mode == open_mode::read_only ? H5F_ACC_RDONLY : H5F_ACC_RDWR,
                H5P_DEFAULT),
  root_group_(*this),
  attributes(root_group_)
{
}

file::file(hdf5::file file_wrapper_)
:file_wrapper_(std::move(file_wrapper_)),
 root_group_(*this),
 attributes(root_group_)
{
}

group& file::root_group()
{
    return root_group_;
}

group file::create_group(const std::string& name)
{
    return root_group_.create_group(name);
}

dataset file::create_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims,int comp_level)
{
    return root_group_.create_dataset(name,datatype,dims,comp_level);
}

scalar_dataset file::create_scalar_dataset(const std::string& name, const type& datatype)
{
    return root_group_.create_scalar_dataset(name,datatype);
}

object file::operator[](const std::string& name)const
{
    return root_group_[name];
}

group file::require_group(const std::string& name)
{
    return root_group_.require_group(name);
}

dataset file::require_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims,
                              int comp_level)
{
    return root_group_.require_dataset(name,datatype,dims,comp_level);
}

scalar_dataset file::require_scalar_dataset(const std::string& name, const type& datatype)
{
    return root_group_.require_scalar_dataset(name,datatype);
}

void file::iterate(const std::function<void(const object&)>& op)
{
    root_group_.iterate(op);
}

object_reference file::ref()const
{
    return root_group_.ref();
}

hid_t file::id() const noexcept
{
    return file_wrapper_.id();
}
}
