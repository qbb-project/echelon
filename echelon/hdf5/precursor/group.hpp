//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_PRECURSOR_GROUP_HPP
#define ECHELON_HDF5_PRECURSOR_GROUP_HPP

#include <echelon/hdf5/precursor/object.hpp>
#include <echelon/hdf5/precursor/property_list.hpp>

#include <string>
#include <hdf5.h>
#include <functional>

namespace echelon
{
namespace hdf5
{
namespace precursor
{
class group
{
public:
    group();
    explicit group(hid_t group_id_);
    explicit group(const object& other);
    group(hid_t loc_id_, const std::string& name_, const property_list& lcpl_,
          const property_list& gcpl_, const property_list& gapl_);
    group(hid_t loc_id_, const std::string& name_, const property_list& gapl_);

    ~group();

    group(const group& other);
    group(group&& other);

    group& operator=(const group& other);
    group& operator=(group&& other);

    hsize_t iterate_links(H5_index_t index_type, H5_iter_order_t order, hsize_t start_index,
                          std::function<herr_t(hid_t, const char*)> op) const;

    void visit_links(H5_index_t index_type, H5_iter_order_t order,
                     std::function<herr_t(hid_t, const char*)> visitor) const;

    void visit_objects(H5_index_t index_type, H5_iter_order_t order,
                       std::function<herr_t(hid_t, const char*)> visitor) const;

    void remove(const std::string& name) const;

    hid_t id() const;

private:
    hid_t group_id_;
};
}
}
}

#endif
