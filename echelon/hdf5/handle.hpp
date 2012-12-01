//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_HANDLE_HPP
#define ECHELON_HDF5_HANDLE_HPP

#include <hdf5.h>

namespace echelon
{

namespace hdf5
{

class attribute;
class dataset;
class dataspace;
class file;
class group;
class object;
class property_list;
class type;

class handle
{
public:
    handle();
    explicit handle(hid_t id_);

    handle(const attribute& other);
    handle(const dataset& other);
    handle(const dataspace& other);
    handle(const file& other);
    handle(const group& other);
    handle(const object& other);
    handle(const property_list& other);
    handle(const type& other);

    handle(const handle& other);
    handle(handle&& other);

    ~handle();

    handle& operator=(const handle& other);
    handle& operator=(handle&& other);

    hid_t id()const;
private:
    hid_t id_;
};

}

}

#endif
