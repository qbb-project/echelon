#include <echelon/hdf5/attribute.hpp>

#include <utility>

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
}

attribute::attribute(hid_t loc_id_, const std::string& name_)
: attribute_id_(H5Aopen(loc_id_, name_.c_str(), H5P_DEFAULT))
{
}

attribute::~attribute()
{
    if (id() > -1)
        H5Aclose(id());
}

attribute::attribute(const attribute& other)
:attribute_id_(other.attribute_id_)
{
    H5Iinc_ref(attribute_id_);
}

attribute::attribute(attribute&& other)
:attribute_id_(other.attribute_id_)
{
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
    H5Awrite(id(),H5Aget_type(id()),value);
}

void attribute::read(void* value)const
{
    H5Aread(id(),H5Aget_type(id()),value);
}

hid_t attribute::id()const
{
    return attribute_id_;
}

}
}
