#ifndef ECHELON_HDF5_DIMENSION_SCALE_HPP
#define ECHELON_HDF5_DIMENSION_SCALE_HPP

#include <echelon/hdf5/dataset.hpp>

#include <string>

#include <hdf5_hl.h>

namespace echelon
{
namespace hdf5
{

class dimension_scale
{
public:
    dimension_scale(hid_t loc_id,const std::string& dataset_name,const type& dtype,
                    const dataspace& space,
                    const std::string& scale_name);

    hid_t id()const;
private:
    dataset underlying_dataset_;
};

void attach_dimension_scale(dimension_scale& dim_scale,dataset& to,unsigned int dimension);

}
}

#endif
