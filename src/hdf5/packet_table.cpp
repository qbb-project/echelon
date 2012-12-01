//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/packet_table.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{

packet_table::packet_table(hid_t loc_id, const char* dset_name, hid_t dtype_id,
                           hsize_t chunk_size, int compression)
: table_id_(H5PTcreate_fl(loc_id, dset_name, dtype_id, chunk_size, compression))
{
}

packet_table::packet_table(hid_t loc_id, const char* dset_name)
: table_id_(H5PTopen(loc_id, dset_name))
{
}

packet_table::~packet_table()
{
    H5PTclose(id());
}

packet_table::packet_table(const packet_table& other)
:table_id_(other.table_id_)
{
    H5Iinc_ref(table_id_);
}

packet_table::packet_table(packet_table&& other)
:table_id_(other.table_id_)
{
    other.table_id_ = -1;
}

packet_table& packet_table::operator=(const packet_table& other)
{
    using std::swap;

    packet_table temp(other);
    swap(*this,temp);

    return *this;
}

packet_table& packet_table::operator=(packet_table&& other)
{
    using std::swap;

    swap(table_id_,other.table_id_);

    return *this;
}

hid_t packet_table::id() const
{
    return table_id_;
}

}
}
