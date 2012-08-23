#ifndef ECHELON_OBJECT_REFERENCE_HPP
#define ECHELON_OBJECT_REFERENCE_HPP

#include <echelon/hdf5/object_reference.hpp>
#include <echelon/hdf5/handle.hpp>
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
                              hdf5::handle any_valid_handle_);

    object operator*()const;

    const hdf5::object_reference& raw_ref()const
    {
        return reference_wrapper_;
    }
private:
    hdf5::object_reference reference_wrapper_;
    hdf5::handle any_valid_handle_;
};

template<>
struct type_lowering_hook<object_reference>
{
    typedef object_reference original_type;
    typedef hdf5::object_reference lowered_type;

    template<typename Sink>
    static lowered_type lower_type(const original_type& value,const Sink& sink)
    {
        return value.raw_ref();
    }

    template<typename Source>
    static original_type raise_type(lowered_type value,const Source& source)
    {
        return object_reference(value,hdf5::handle(source));
    }
};

}

#endif
