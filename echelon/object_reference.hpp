#ifndef ECHELON_OBJECT_REFERENCE_HPP
#define ECHELON_OBJECT_REFERENCE_HPP

#include <echelon/hdf5/object_reference.hpp>
#include <echelon/customization_hooks.hpp>
#include <echelon/object.hpp>

#include <string>

namespace echelon
{

class object_reference
{
public:
    friend struct type_lowering_hook<object_reference>;

    //fast workaround!
    //only needed for reading datasets of object_references by now;
    //should not be used for anything different and should be
    //deleted after we fixed this problem
    object_reference() = default;

    explicit object_reference(const object& referenced_object);
    explicit object_reference(const hdf5::object_reference& reference_wrapper_,
                              object any_valid_object_);

    object operator*()const;

    const hdf5::object_reference& raw_ref()const
    {
        return reference_wrapper_;
    }
private:
    hdf5::object_reference reference_wrapper_;
    object any_valid_object_;
};

}

#endif
