#include <echelon/hdf5/attribute.hpp>

#include <utility>

#include <echelon/hdf5/error_handling.hpp>

namespace echelon
{
namespace hdf5
{

attribute::attribute(hid_t loc_id_, const std::string& name_,
                     const type& attr_type_)
:
attribute_id_(H5Acreate2(loc_id_, name_.c_str(), attr_type_.id(), dataspace().id(),
                         H5P_DEFAULT, H5P_DEFAULT))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

attribute::attribute(hid_t loc_id_, const std::string& name_)
: attribute_id_(H5Aopen(loc_id_, name_.c_str(), H5P_DEFAULT))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

attribute::~attribute()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

        ECHELON_VERIFY_MSG(H5Aclose(id()) >= 0,"unable to close the attribute");
    }
}

attribute::attribute(const attribute& other)
:attribute_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

attribute::attribute(attribute&& other)
:attribute_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    other.attribute_id_ = -1;
}

attribute& attribute::operator=(const attribute& other)
{
    using std::swap;

    attribute temp(other);
    swap(*this,temp);

    return *this;
}

attribute& attribute::operator=(attribute&& other)
{
    using std::swap;

    swap(attribute_id_,other.attribute_id_);

    return *this;
}

void attribute::write(const void* value)
{
    if(H5Awrite(id(),H5Aget_type(id()),value) < 0)
        throw_on_hdf5_error();
}

void attribute::read(void* value)const
{
    if(H5Aread(id(),H5Aget_type(id()),value) < 0)
        throw_on_hdf5_error();
}

hid_t attribute::id()const
{
    return attribute_id_;
}

}
}
