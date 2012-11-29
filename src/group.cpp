#include <echelon/group.hpp>

#include <echelon/file.hpp>

#include <iostream>

namespace echelon
{

group::group(const file& loc, const std::string& name)
:group_wrapper_(loc.id(), name, H5P_DEFAULT),attributes(*this)
{}

group::group(const object& parent, const std::string& name, creation_mode mode)
:group_wrapper_(mode == creation_mode::create ? hdf5::group(parent.id(), name, H5P_DEFAULT, H5P_DEFAULT,
                                                            H5P_DEFAULT)
                                              : hdf5::group(parent.id(), name, H5P_DEFAULT)
                  ),
 attributes(*this)
{}

group::group(hdf5::group group_wrapper_)
:group_wrapper_(std::move(group_wrapper_)),attributes(*this)
{
}

group group::create_group(const std::string& name)
{
    return group(object(*this), name, creation_mode::create);
}

dataset group::create_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims,int comp_level)
{
    return dataset(object(*this),name,datatype,dims,comp_level);
}

scalar_dataset group::create_scalar_dataset(const std::string& name, const type& datatype)
{
    return scalar_dataset(object(*this),name,datatype);
}

object group::operator[](const std::string& name)const
{
    return object(hdf5::object(id(),name));
}

group group::require_group(const std::string& name)
{
    if(exists(object(*this),name) && get_object_type_by_name(object(*this),name) == object_type::group)
    {
        return group(hdf5::group(id(),name,H5P_DEFAULT));
    }
    else
    {
        return create_group(name);
    }
}

dataset group::require_dataset(const std::string& name, const type& datatype,
                               const std::vector<hsize_t>& dims,
                               int comp_level)
{
    if(exists(object(*this),name) && get_object_type_by_name(object(*this),name) == object_type::dataset)
    {
        dataset ds(hdf5::dataset(id(),name,hdf5::default_property_list));

        if(ds.shape() != dims)
            throw broken_contract_exception("The required shape doesn't "
                                            "match the shape of the dataset.");

        if(ds.datatype() != datatype)
            throw broken_contract_exception("The required datatype doesn't "
                                            "match the datatype of the dataset.");

        return ds;
    }
    else
    {
        return create_dataset(name,datatype,dims,comp_level);
    }
}

scalar_dataset group::require_scalar_dataset(const std::string& name, const type& datatype)
{   
    if(exists(object(*this),name) && get_object_type_by_name(object(*this),name) == object_type::scalar_dataset)
    {
        scalar_dataset ds(hdf5::dataset(id(),name,hdf5::default_property_list));

        if(ds.datatype() != datatype)
            throw broken_contract_exception("The required datatype doesn't "
                                            "match the datatype of the dataset.");

        return ds;
    }
    else
    {
        return create_scalar_dataset(name,datatype);
    }
}

void group::iterate(const std::function<void(const object&)>& op)
{
    group_wrapper_.iterate(id(),H5_INDEX_NAME,H5_ITER_NATIVE,0,
                           [&op](hid_t loc_id, const char* name) -> hid_t
                           {
                               op(object(hdf5::object(loc_id,name)));

                               return 0;
                           }
                          );
}

object_reference group::ref()const
{
    return object_reference(object(*this));
}

hid_t group::id() const noexcept
{
    return group_wrapper_.id();
}

const hdf5::group& group::get_native_handle()const
{
    return group_wrapper_;
}

}
