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

namespace detail
{

template<typename T>
struct attribute_writer
{
    static void write(hid_t attribute_id,hid_t datatype_id,const T& value)
    {
        H5Awrite(attribute_id,datatype_id,&value);
    }
};

template<int Size>
struct attribute_writer<char[Size]>
{
    static void write(hid_t attribute_id,hid_t datatype_id,const char value[Size])
    {
        const char* str = value;

        H5Awrite(attribute_id,datatype_id,&str);
    }
};

template<typename T>
struct attribute_reader
{
    static void read(hid_t attribute_id,hid_t datatype_id,T& value)
    {
        H5Aread(attribute_id,datatype_id,&value);
    }
};

template<int Size>
struct attribute_reader<char[Size]>
{
    static void read(hid_t attribute_id,hid_t datatype_id,char value[Size])
    {
        char* str = value;

        H5Aread(attribute_id,datatype_id,&str);
    }
};

}

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
    
    void write(const void* value)
    {
        /*static_assert(std::is_standard_layout<T>::value  ||
                      std::is_same<T,std::string>::value
                      ,"T must be a standard layout type"
                      "or a string");*/

        //detail::attribute_writer<T>::write(id(),H5Aget_type(id()),value);
        H5Awrite(id(),H5Aget_type(id()),value);
    }

    void read(void* value)const
    {
        /*static_assert(std::is_standard_layout<T>::value  ||
                      std::is_same<T,std::string>::value
                      ,"T must be a standard layout type"
                      "or a string");*/

        //detail::attribute_reader<T>::read(id(),H5Aget_type(id()),value);
        H5Aread(id(),H5Aget_type(id()),value);
    }

    hid_t id()const;
private:
    hid_t attribute_id_;
};

}
}

#endif
