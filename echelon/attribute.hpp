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
    attribute& operator=(const T& value)
    {
        write(attribute_wrapper_,value);

        return *this;
    }

    template<typename T>
    operator T()const
    {
        T value;

        read(attribute_wrapper_,value);

        return value;
    }

    hid_t id()const;
private:
    hdf5::attribute attribute_wrapper_;
};

}

#endif
