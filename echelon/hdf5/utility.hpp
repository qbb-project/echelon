//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_UTILITY_HPP
#define ECHELON_HDF5_UTILITY_HPP

#include <echelon/hdf5/object.hpp>

#include <string>

#include <hdf5.h> //for hid_t

namespace echelon
{
namespace hdf5
{
enum class object_type
{
    group,
    dataset,
    scalar_dataset,
    unknown
};

object_type get_object_type(hid_t obj_id);

object_type get_object_type_by_name(const object& loc, const std::string& name);

bool exists(const object& loc, const std::string& name);
}
}

#endif
