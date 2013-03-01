#include <echelon/dimension_scale.hpp>

#include <echelon/dataset.hpp>
#include <echelon/file.hpp>
#include <echelon/type.hpp>
#include <echelon/uuid.hpp>

namespace echelon
{

dimension_scale::dimension_scale(const dataset& associated_dataset,
                                 const std::string& dataset_name,
                                 const type& datatype,
                                 const std::vector<hsize_t>& extent,
                                 const std::string& scale_name)
{
    file associated_file(associated_dataset.get_native_handle().associated_file());

    auto echelon_group = associated_file.require_group("echelon");
    auto dimension_scales_group = echelon_group.require_group("dimension_scales");

    std::string unique_id = generate_unique_identifier(associated_dataset.get_native_handle().name());

    auto this_datasets_dimension_scales = dimension_scales_group.require_group(unique_id);

    hdf5::dataspace space(extent);

    dim_scale_ = hdf5::dimension_scale(this_datasets_dimension_scales.id(),dataset_name,
                                       datatype.get_native_type(),space,scale_name);
}

std::vector<hsize_t> dimension_scale::shape()const
{
    return dim_scale_.get_space().get_simple_extent_dims();
}

hdf5::dimension_scale& dimension_scale::get_native_handle()
{
    return dim_scale_;
}

}
