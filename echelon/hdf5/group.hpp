//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_RAII_GROUP_HPP
#define ECHELON_RAII_GROUP_HPP

#include <echelon/hdf5/object.hpp>
#include <echelon/hdf5/property_list.hpp>

#include <string>
#include <hdf5.h>
#include <functional>

namespace echelon
{
namespace hdf5
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

    hsize_t iterate(H5_index_t index_type, H5_iter_order_t order,
                    hsize_t start_index, std::function<herr_t(hid_t,const char*)> op,
                    bool ignore_internal_groups = true)const;

    hid_t id()const;
private:
    hid_t group_id_;
};

}
}

#endif
