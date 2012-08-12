#include <echelon/hdf5/property_list.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{

property_list_class::property_list_class(hid_t class_id_)
: class_id_(class_id_)
{
}

hid_t property_list_class::id() const
{
    return class_id_;
}

property_list::property_list(hid_t property_list_id_)
: property_list_id_(property_list_id_)
{
    if(property_list_id_ != H5P_DEFAULT)
        H5Iinc_ref(property_list_id_);
}

property_list::property_list(property_list_class cls_)
: property_list_id_(H5Pcreate(cls_.id()))
{
}

property_list::~property_list()
{
    if (id() > -1)
        H5Pclose(id());
}

property_list::property_list(const property_list& other)
:property_list_id_(other.property_list_id_)
{
    H5Iinc_ref(property_list_id_);
}

property_list::property_list(property_list&& other)
:property_list_id_(other.property_list_id_)
{
    other.property_list_id_ = -1;
}

property_list& property_list::operator=(const property_list& other)
{
    using std::swap;

    property_list temp(other);
    swap(*this,temp);

    return *this;
}

property_list& property_list::operator=(property_list&& other)
{
    using std::swap;

    swap(property_list_id_,other.property_list_id_);

    return *this;
}

void property_list::set_chunk(const std::vector<hsize_t>& dims)
{
    H5Pset_chunk(id(), dims.size(), dims.data());
}

void property_list::set_deflate(unsigned int level)
{
    H5Pset_deflate(id(), level);
}

hid_t property_list::id() const
{
    return property_list_id_;
}

}
}
