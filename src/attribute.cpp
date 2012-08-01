#include <echelon/attribute.hpp>

namespace echelon
{

attribute::attribute(const object& parent, const std::string& name,
                     const type& datatype)
:attribute_wrapper_(parent.id(),name,datatype.get_native_type())
{}

attribute::attribute(const object& parent, const std::string& name)
:attribute_wrapper_(parent.id(),name)
{}

hid_t attribute::id()const
{
    return attribute_wrapper_.id();
}

}
