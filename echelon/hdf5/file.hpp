//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_FILE_HPP
#define ECHELON_HDF5_FILE_HPP

#include <string>
#include <hdf5.h>

namespace echelon
{
namespace hdf5
{

class file
{
public:
    file(const std::string& name_, unsigned flags_, hid_t fcpl_id_,
         hid_t fapl_id_);
    file(const std::string& name_, unsigned flags_, hid_t fapl_id_);

    ~file();

    file(const file& other);
    file(file&& other);

    file& operator=(const file& other);
    file& operator=(file&& other);

    hid_t id()const noexcept;
private:
    hid_t file_id_;
};

}
}

#endif
