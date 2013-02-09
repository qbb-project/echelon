//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_ATTRIBUTE_HPP
#define ECHELON_HDF5_ATTRIBUTE_HPP

#include <echelon/hdf5/object_reference.hpp>
#include <echelon/hdf5/object.hpp>

#include <hdf5.h>
#include <string>
#include <type_traits>

#include "type.hpp"
#include "dataspace.hpp"
#include "property_list.hpp"

namespace echelon
{
namespace hdf5
{

class attribute
{
public:
    attribute(hid_t loc_id_, const std::string& name_, const type& attr_type_);
    attribute(hid_t loc_id_, const std::string& name_);
    ~attribute();
    attribute(const attribute& other);
    attribute(attribute&& other);

    attribute& operator=(const attribute& other);
    attribute& operator=(attribute&& other);
    
    void write(const void* value);
    void read(void* value)const;

    type datatype()const;

    hid_t id()const;
private:
    hid_t attribute_id_;
};

bool is_attribute_existing(const object& loc,const std::string& name);

}
}

#endif
