#ifndef ECHELON_RAII_ATTRIBUTE_HPP
#define ECHELON_RAII_ATTRIBUTE_HPP

#include <echelon/hdf5/object_reference.hpp>

#include <hdf5.h>
#include <string>
#include <type_traits>

#include "type.hpp"
#include "dataspace.hpp"
#include "property_list.hpp"

namespace echelon
{
namespace hdf5
{

class attribute
{
public:
    attribute(hid_t loc_id_, const std::string& name_, const type& attr_type_);
    attribute(hid_t loc_id_, const std::string& name_);
    ~attribute();
    attribute(const attribute& other);
    attribute(attribute&& other);

    attribute& operator=(const attribute& other);
    attribute& operator=(attribute&& other);
    
    void write(const void* value);
    void read(void* value)const;

    hid_t id()const;
private:
    hid_t attribute_id_;
};

}
}

#endif
