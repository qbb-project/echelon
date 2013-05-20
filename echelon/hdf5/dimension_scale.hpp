//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_DIMENSION_SCALE_HPP
#define ECHELON_HDF5_DIMENSION_SCALE_HPP

#include <echelon/hdf5/dataset.hpp>

#include <string>

#include <hdf5.h>
#include <hdf5_hl.h>

namespace echelon
{
namespace hdf5
{

class dimension_scale
{
public:
    dimension_scale() = default;
    dimension_scale(hid_t loc_id,const std::string& dataset_name,const type& dtype,
                    const dataspace& space,
                    const std::string& scale_name);

    void write(const type& mem_type, const dataspace& mem_space, const dataspace& file_space,
               const property_list& xfer_plist, const void* buf);
    void read(const type& mem_type, const dataspace& mem_space, const dataspace& file_space,
              const property_list& xfer_plist, void* buf)const;

    void write(const void* value);
    void read(void* value)const;

    dataspace get_space()const;
    type datatype()const;

    hid_t id()const;
private:
    dataset underlying_dataset_;
};

void attach_dimension_scale(dimension_scale& dim_scale,dataset& to,unsigned int dimension);

}
}

#endif
