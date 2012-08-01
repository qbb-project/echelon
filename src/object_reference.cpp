#include <echelon/object_reference.hpp>

#include <echelon/object.hpp>
#include <echelon/group.hpp>
#include <echelon/dataset.hpp>

namespace echelon
{

object_reference::object_reference(const object& referenced_object)
:reference_wrapper_(referenced_object.id()),any_valid_object_(referenced_object)
{}

object_reference::object_reference(const hdf5::object_reference& reference_wrapper_,
                                   object any_valid_object_)
:reference_wrapper_(reference_wrapper_),any_valid_object_(any_valid_object_)
{}

object object_reference::operator*()const
{
    return object(reference_wrapper_.dereference(any_valid_object_.id()));
}

}
