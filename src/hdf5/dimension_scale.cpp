//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/dimension_scale.hpp>

#include <echelon/hdf5/error_handling.hpp>

namespace echelon
{
namespace hdf5
{

dimension_scale::dimension_scale(hid_t loc_id, const std::string& dataset_name,
                                 const type& dtype, const dataspace& space,
                                 const std::string& scale_name)
: underlying_dataset_(loc_id, dataset_name, dtype, space, default_property_list,
                      default_property_list, default_property_list)
{
    if (H5DSset_scale(id(), scale_name.c_str()) < 0)
        throw_on_hdf5_error();
}

void dimension_scale::write(const type& mem_type, const dataspace& mem_space,
                            const dataspace& file_space,
                            const property_list& xfer_plist, const void* buf)
{
    underlying_dataset_.write(mem_type, mem_space, file_space, xfer_plist, buf);
}

void dimension_scale::read(const type& mem_type, const dataspace& mem_space,
                           const dataspace& file_space,
                           const property_list& xfer_plist, void* buf) const
{
    underlying_dataset_.read(mem_type, mem_space, file_space, xfer_plist, buf);
}

void dimension_scale::write(const void* value)
{
    underlying_dataset_.write(value);
}

void dimension_scale::read(void* value) const
{
    underlying_dataset_.read(value);
}

dataspace dimension_scale::get_space() const
{
    return underlying_dataset_.get_space();
}

type dimension_scale::datatype() const
{
    return underlying_dataset_.datatype();
}

hid_t dimension_scale::id() const
{
    return underlying_dataset_.id();
}

void attach_dimension_scale(dimension_scale& dim_scale, dataset& to,
                            unsigned int dimension)
{
    if (H5DSattach_scale(to.id(), dim_scale.id(), dimension) < 0)
        throw_on_hdf5_error();
}
}
}
