#include <echelon/dataset_dimensions.hpp>

#include <echelon/dataset.hpp>
#include <echelon/hdf5/dimension_scale.hpp>
#include <echelon/file.hpp>
#include <echelon/uuid.hpp>

#include <cassert>

namespace echelon
{

dimension::dimension(dataset& associated_dataset_,std::size_t index_)
:associated_dataset_(&associated_dataset_),index_(index_)
{
}

void dimension::attach_dimension_scale(const std::string& name,const type& datatype)
{
    file associated_file(associated_dataset_->get_native_handle().associated_file());

    auto echelon_group = associated_file.require_group("echelon");
    auto dimension_scales_group = echelon_group.require_group("dimension_scales");

    std::string unique_id = generate_unique_identifier(associated_dataset_->get_native_handle().name());

    auto this_datasets_dimension_scales = dimension_scales_group.require_group(unique_id);

    std::vector<hsize_t> extent = { extend() };
    hdf5::dataspace space(extent);

    hdf5::dimension_scale dim_scale(this_datasets_dimension_scales.id(),"dim" + std::to_string(index_),
                                    datatype.get_native_type(),space,name);

    hdf5::attach_dimension_scale(dim_scale,associated_dataset_->get_native_handle(),index_);
}

std::string dimension::label()const
{
   return associated_dataset_->get_native_handle().label(index_);
}

void dimension::relabel(const std::string& new_label)
{
    associated_dataset_->get_native_handle().relabel(index_,new_label);
}

hsize_t dimension::extend()const
{
    return associated_dataset_->get_native_handle().get_space().get_simple_extent_dims()[index_];
}

dataset_dimensions::dataset_dimensions(dataset& associated_dataset_,std::size_t rank_)
{
    for(std::size_t i = 0;i < rank_;++i)
        dimensions_.emplace_back(associated_dataset_,i);

    assert(rank_ == dimensions_.size());
}

}
