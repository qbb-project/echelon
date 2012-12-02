//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/dynamic_table.hpp>

#include <echelon/hdf5/property_list.hpp>

namespace echelon
{
namespace hdf5
{

dynamic_table::dynamic_table(hid_t loc_id, const std::string& dset_name,
                             const type& dtype, hsize_t chunk_size,
                             int compression)
: table_dataset_(-1)
{
    hsize_t size = 0;
    hsize_t max_size = H5S_UNLIMITED;
    dataspace space({ size },{ max_size });

    property_list creation_property_list(
    property_list_class(H5P_DATASET_CREATE));

    creation_property_list.set_chunk({ chunk_size });
    if (compression > -1)
        creation_property_list.set_deflate(compression);

    table_dataset_ = dataset(loc_id, dset_name, dtype, space,
                             default_property_list, creation_property_list,
                             default_property_list);
}

dynamic_table::dynamic_table(hid_t loc_id, const std::string& dset_name)
: table_dataset_(loc_id, dset_name, default_property_list)
{
}

void dynamic_table::erase(hsize_t index)
{
    dataspace file_space = table_dataset_.get_space();
    type datatype = table_dataset_.get_type();

    std::vector<hsize_t> num_of_packets = file_space.get_simple_extent_dims();

    std::vector<hsize_t> moved_range_size = { num_of_packets[0] - (index + 1) };
    dataspace mem_space(moved_range_size);

    std::vector<char> buffer(moved_range_size[0] * datatype.size());

    hsize_t start_of_moved_range = index + 1;
    file_space.select_hyperslab(H5S_SELECT_SET,
                                { start_of_moved_range },
                                moved_range_size);

    table_dataset_.read(datatype, mem_space, file_space, default_property_list,
                        buffer.data());

    file_space.select_hyperslab(H5S_SELECT_SET,
                                { index },
                                moved_range_size);

    table_dataset_.write(datatype, mem_space, file_space, default_property_list,
                         buffer.data());

    --num_of_packets[0];
    table_dataset_.set_extent(num_of_packets);
}

void dynamic_table::detach()
{
    dataspace file_space = table_dataset_.get_space();

    std::vector<hsize_t> num_of_packets = file_space.get_simple_extent_dims();

    --num_of_packets[0];
    table_dataset_.set_extent(num_of_packets);
}

hid_t dynamic_table::id() const
{
    return table_dataset_.id();
}

}
}
