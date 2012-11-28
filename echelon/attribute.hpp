#ifndef ECHELON_ATTRIBUTE_HPP
#define ECHELON_ATTRIBUTE_HPP

#include <echelon/type.hpp>
#include <echelon/object.hpp>

#include <echelon/hdf5/attribute.hpp>

#include <echelon/data_transfer_broker.hpp>

#include <utility>

namespace echelon
{

class attribute
{
public:
    attribute(const object& parent, const std::string& name,
              const type& datatype);
    attribute(const object& parent, const std::string& name);

    template<typename T>
    friend inline void operator<<=(attribute& attr,const T& value)
    {
        write(attr.attribute_wrapper_,value);
    }

    template<typename T>
    friend inline void operator<<=(T& value,const attribute& attr)
    {
        read(attr.attribute_wrapper_,value);
    }

    type datatype()const;

    hid_t id()const;
    const hdf5::attribute& get_native_handle()const;
private:
    hdf5::attribute attribute_wrapper_;
};

}

#endif
