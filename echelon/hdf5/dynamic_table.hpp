//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_RAII_DYNAMIC_TABLE_HPP
#define ECHELON_RAII_DYNAMIC_TABLE_HPP

#include "type.hpp"
#include "dataset.hpp"
#include "dataspace.hpp"

#include <hdf5.h>
#include <string>
#include <vector>

namespace echelon
{
namespace hdf5
{

class dynamic_table
{
public:
    dynamic_table(hid_t loc_id, const std::string& dset_name, const type& dtype,
                  hsize_t chunk_size, int compression);
    dynamic_table(hid_t loc_id, const std::string& dset_name);

    template<typename T>
    void append(const T& value)
    {
        dataspace file_space = table_dataset_.get_space();

        std::vector<hsize_t> num_of_packets =
        file_space.get_simple_extent_dims();

        hsize_t index = num_of_packets[0]++;

        table_dataset_.set_extent(num_of_packets);

        file_space.select_elements(H5S_SELECT_SET, 1,
            { index });

        type datatype = table_dataset_.get_type();

        dataspace mem_space;

        table_dataset_.write(datatype, mem_space, file_space,
                             default_property_list, &value);
    }

    template<typename T>
    T get(hsize_t index)
    {
        T value;

        dataspace file_space = table_dataset_.get_space();
        file_space.select_elements(H5S_SELECT_SET, 1,
            { index });

        type datatype = table_dataset_.get_type();

        dataspace mem_space;

        table_dataset_.read(datatype, mem_space, file_space,
                            default_property_list, &value);

        return value;
    }

    void erase(hsize_t index);
    void detach();

    hid_t id() const;
    private:
    dataset table_dataset_;
};

}
}

#endif
