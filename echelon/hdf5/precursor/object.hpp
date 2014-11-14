//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_OBJECT_HPP
#define ECHELON_HDF5_PRECURSOR_OBJECT_HPP

#include <hdf5.h>
#include <string>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
class dataset;
class group;

struct share_ownership_t
{
};

static constexpr share_ownership_t share_ownership = {};

class object
{
public:
    object();

    explicit object(hid_t object_id_);
    object(hid_t object_id_, share_ownership_t);
    object(hid_t loc_id_, const std::string& name);

    explicit object(const dataset& other);
    explicit object(const group& other);

    object(const object& other);
    object(object&& other);

    ~object();

    object& operator=(const object& other);
    object& operator=(object&& other);

    hid_t id() const;

    explicit operator bool() const;
private:
    hid_t object_id_;
};

bool exists(const object& loc, const std::string& name);
std::string get_name(const object& loc);
}
}
}

#endif
