//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_FILE_HPP
#define ECHELON_HDF5_PRECURSOR_FILE_HPP

#include <echelon/hdf5/precursor/group.hpp>

#include <string>
#include <hdf5.h>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
class file
{
public:
    file();
    explicit file(hid_t file_id_);
    file(const std::string& name_, unsigned flags_, hid_t fcpl_id_, hid_t fapl_id_);
    file(const std::string& name_, unsigned flags_, hid_t fapl_id_);

    ~file();

    file(const file& other);
    file(file&& other);

    file& operator=(const file& other);
    file& operator=(file&& other);

    hid_t id() const noexcept;

    explicit operator bool() const;
private:
    hid_t file_id_;
};

group mount(const file& mounted_file, const group& mount_point);
group mount(const file& mounted_file, const file& mount_point);

void unmount(const group& mount_point);
}
}
}

#endif
