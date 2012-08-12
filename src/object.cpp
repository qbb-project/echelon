#include <echelon/object.hpp>

#include <echelon/dataset.hpp>
#include <echelon/group.hpp>
#include <echelon/scalar_dataset.hpp>
#include <echelon/object_reference.hpp>

#include <echelon/utility.hpp>

#include <utility>
#include <cassert>

namespace echelon
{

object::object()
:object_id_(-1)
{}

object::object(hid_t object_id_)
:object_id_(object_id_)
{
    assert(H5Iis_valid(object_id_));
    H5Iinc_ref(id());
}

object::object(hid_t loc_id_,const std::string& name)
:object_id_(H5Oopen(loc_id_,name.c_str(),H5P_DEFAULT))
{}

object::object(const group& object_)
:object_id_(object_.id())
{
    H5Iinc_ref(id());
}

object::object(const dataset& object_)
:object_id_(object_.id())
{
    H5Iinc_ref(id());
}

object::object(const scalar_dataset& object_)
:object_id_(object_.id())
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

object& object::operator=(const group& object_)
{
    using std::swap;

    object temp(object_);
    std::swap(*this,temp);

    return *this;
}

object& object::operator=(const dataset& object_)
{
    using std::swap;

    object temp(object_);
    std::swap(*this,temp);

    return *this;
}

object& object::operator=(const scalar_dataset& object_)
{
    using std::swap;

    object temp(object_);
    std::swap(*this,temp);

    return *this;
}

object::operator group()const
{
    if(get_object_type(id()) != object_type::group)
        throw wrong_object_type_exception("wrong object type");

    return group(hdf5::group(id()));
}

object::operator dataset()const
{
    if(get_object_type(id()) != object_type::dataset)
        throw wrong_object_type_exception("wrong object type");

    return dataset(hdf5::dataset(id()));
}

object::operator scalar_dataset()const
{
    if(get_object_type(id()) != object_type::scalar_dataset)
        throw wrong_object_type_exception("wrong object type");

    return scalar_dataset(hdf5::dataset(id()));
}

object_reference object::ref()const
{
    return object_reference(*this);
}

hid_t object::id()const
{
    return object_id_;
}

}
