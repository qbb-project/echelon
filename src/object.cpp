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

object::object(const hdf5::object& object_wrapper_)
:object_wrapper_(object_wrapper_)
{
}

object::object(const object& other)
:object_wrapper_(other.object_wrapper_)
{
}

object::object(const group& object_)
:object_wrapper_(object_.get_native_handle())
{
}

object::object(const dataset& object_)
:object_wrapper_(object_.get_native_handle())
{
}

object::object(const scalar_dataset& object_)
:object_wrapper_(object_.get_native_handle())
{
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

    return group(hdf5::group(object_wrapper_));
}

object::operator dataset()const
{
    if(get_object_type(id()) != object_type::dataset)
        throw wrong_object_type_exception("wrong object type");

    return dataset(hdf5::dataset(object_wrapper_));
}

object::operator scalar_dataset()const
{
    if(get_object_type(id()) != object_type::scalar_dataset)
        throw wrong_object_type_exception("wrong object type");

    return scalar_dataset(hdf5::dataset(object_wrapper_));
}

object_reference object::ref()const
{
    return object_reference(*this);
}

hid_t object::id()const
{
    return object_wrapper_.id();
}

const hdf5::object& object::get_native_handle()const
{
    return object_wrapper_;
}

}
