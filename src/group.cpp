#include <echelon/group.hpp>

#include <echelon/file.hpp>
#include <echelon/utility.hpp>

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
