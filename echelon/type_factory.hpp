#ifndef ECHELON_TYPE_FACTORY_HPP
#define ECHELON_TYPE_FACTORY_HPP

#include "type.hpp"
#include "type_traits.hpp"
#include <utility>
#include <string>
#include <complex>

#include <echelon/object_reference.hpp>

#include "hdf5_type_oarchive.hpp"

namespace echelon
{

template<typename T>
struct hdf5_type_selector
{
    static_assert(sizeof(T) != sizeof(T),"invalid type");
};

template<>
struct hdf5_type_selector<char>
{
    static type get()
    {
        return type::char_();
    }
};

template<>
struct hdf5_type_selector<unsigned char>
{
    static type get()
    {
        return type::uchar();
    }
};

template<>
struct hdf5_type_selector<short>
{
    static type get()
    {
        return type::short_();
    }
};

template<>
struct hdf5_type_selector<unsigned short>
{
    static type get()
    {
        return type::ushort();
    }
};

template<>
struct hdf5_type_selector<int>
{
    static type get()
    {
        return type::int_();
    }
};

template<>
struct hdf5_type_selector<unsigned int>
{
    static type get()
    {
        return type::uint();
    }
};

template<>
struct hdf5_type_selector<long>
{
    static type get()
    {
        return type::long_();
    }
};

template<>
struct hdf5_type_selector<unsigned long>
{
    static type get()
    {
        return type::ulong();
    }
};

template<>
struct hdf5_type_selector<long long>
{
    static type get()
    {
        return type::long_long();
    }
};

template<>
struct hdf5_type_selector<unsigned long long>
{
    static type get()
    {
        return type::ulong_long();
    }
};

template<>
struct hdf5_type_selector<float>
{
    static type get()
    {
        return type::float_();
    }
};

template<>
struct hdf5_type_selector<double>
{
    static type get()
    {
        return type::double_();
    }
};

template<>
struct hdf5_type_selector<bool>
{
    static type get()
    {
        return type::uchar();
    }
};

template<>
struct hdf5_type_selector<std::string>
{
    static type get()
    {
        return type::string();
    }
};

template<typename T>
struct hdf5_type_selector<std::complex<T>>
{
    static type get()
    {
        type_layout layout(sizeof(std::complex<T>));

        type scalar_type = get_hdf5_type<T>();

        layout.add_element("real",scalar_type,0);
        layout.add_element("imag",scalar_type,sizeof(T));

        return type::compound_type(layout);
    }
};

template<>
struct hdf5_type_selector<object_reference>
{
    static type get()
    {
        return type::object_reference();
    }
};

template<typename T>
struct hdf5_type_selector<T const>
: hdf5_type_selector<typename std::remove_cv<T>::type>
{
};

template<typename T>
inline typename std::enable_if<is_predefined_hdf5_type<T>::value, type>::type
get_hdf5_type()
{
    return hdf5_type_selector<T>::get();
}

template<typename T>
inline typename std::enable_if<is_hdf5_type<T>::value &&
                               !is_predefined_hdf5_type<T>::value, type>::type
get_hdf5_type()
{
    typedef typename std::remove_reference<typename std::remove_cv<T>::type>::type value_type;

    value_type value;
    hdf5_type_oarchive type_oarchive(value);

    type_oarchive & boost::serialization::make_nvp(typeid(T).name(), value);

    return type_oarchive.get_result_type();
}

template<typename T>
inline typename std::enable_if<!is_hdf5_type<T>::value, type>::type
get_hdf5_type()
{
    static_assert(is_hdf5_type<T>::value,"T must be a valid HDF5 type");
}
}

#endif
