//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_ATTRIBUTE_HPP
#define ECHELON_HDF5_PRECURSOR_ATTRIBUTE_HPP

#include <echelon/hdf5/precursor/object_reference.hpp>
#include <echelon/hdf5/precursor/object.hpp>
#include <echelon/hdf5/precursor/property_list.hpp>
#include <echelon/hdf5/precursor/type.hpp>
#include <echelon/hdf5/precursor/dataspace.hpp>
#include <echelon/hdf5/precursor/property_list.hpp>

#include <hdf5.h>
#include <string>
#include <type_traits>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
class attribute
{
public:
    explicit attribute(hid_t attribute_id_);
    attribute(hid_t loc_id_, const std::string& name_, const type& attr_type_,
              const property_list& acpl, const property_list& aapl);
    attribute(hid_t loc_id_, const std::string& name_);
    ~attribute();
    attribute(const attribute& other);
    attribute(attribute&& other);

    attribute& operator=(const attribute& other);
    attribute& operator=(attribute&& other);

    void write(const void* value);
    void read(void* value) const;

    type datatype() const;

    hid_t id() const;

private:
    hid_t attribute_id_;
};

bool is_attribute_existing(const object& loc, const std::string& name);
}
}
}

#endif
