#include <echelon/hdf5/dimension_scale.hpp>

#include <echelon/hdf5/error_handling.hpp>

namespace echelon
{
namespace hdf5
{

dimension_scale::dimension_scale(hid_t loc_id,const std::string& dataset_name,const type& dtype,
                                 const dataspace& space,
                                 const std::string& scale_name)
:underlying_dataset_(loc_id, dataset_name, dtype,
                     space,
                     default_property_list,
                     default_property_list,
                     default_property_list)
{
    if(H5DSset_scale(id(),scale_name.c_str()) < 0)
        throw_on_hdf5_error();
}

hid_t dimension_scale::id()const
{
    return underlying_dataset_.id();
}

void attach_dimension_scale(dimension_scale& dim_scale,dataset& to,unsigned int dimension)
{
    if(H5DSattach_scale(to.id(),dim_scale.id(),dimension) < 0)
        throw_on_hdf5_error();
}

}
}
