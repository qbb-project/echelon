#include <echelon/hdf5/object.hpp>

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/group.hpp>

#include <cassert>
#include <utility>

namespace echelon
{

namespace hdf5
{

object::object()
:object_id_(-1)
{}

object::object(hid_t object_id_)
:object_id_(object_id_)
{
    assert(H5Iis_valid(object_id_));
}

object::object(hid_t loc_id_,const std::string& name)
:object_id_(H5Oopen(loc_id_,name.c_str(),H5P_DEFAULT))
{}

object::object(const dataset& other)
:object_id_(other.id())
{
    H5Iinc_ref(id());
}

object::object(const group& other)
:object_id_(other.id())
{
    H5Iinc_ref(id());
}

object::object(const object& other)
:object_id_(other.object_id_)
{
    H5Iinc_ref(id());
}

object::object(object&& other)
:object_id_(other.object_id_)
{
    other.object_id_ = -1;
}

object::~object()
{
    if(id() > -1)
        H5Oclose(id());
}

object& object::operator=(const object& other)
{
    using std::swap;

    object temp(other);
    std::swap(*this,temp);

    return *this;
}

object& object::operator=(object&& other)
{
    using std::swap;

    swap(object_id_,other.object_id_);

    return *this;
}

hid_t object::id()const
{
    return object_id_;
}

}
}
