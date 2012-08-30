#include <echelon/hdf5/object.hpp>

#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/group.hpp>

#include <cassert>
#include <utility>

#include <echelon/hdf5/error_handling.hpp>

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
    ECHELON_ASSERT_MSG(id() == -1 || H5Iget_type(id()) == H5I_DATASET,
                       "ID does not refer to a dataset");
}

object::object(hid_t loc_id_,const std::string& name)
:object_id_(H5Oopen(loc_id_,name.c_str(),H5P_DEFAULT))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

object::object(const dataset& other)
:object_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

object::object(const group& other)
:object_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

object::object(const object& other)
:object_id_(other.object_id_)
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

object::object(object&& other)
:object_id_(other.object_id_)
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    other.object_id_ = -1;
}

object::~object()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

        ECHELON_VERIFY_MSG(H5Oclose(id()) >= 0,"unable to close the object");
    }
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