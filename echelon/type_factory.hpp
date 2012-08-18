#ifndef ECHELON_TYPE_FACTORY_HPP
#define ECHELON_TYPE_FACTORY_HPP

#include "type.hpp"
#include "type_traits.hpp"
#include <utility>
#include <string>

#include <echelon/object_reference.hpp>

#include <echelon/static_type_layout.hpp>

namespace echelon
{

template<typename T>
inline typename std::enable_if<is_predefined_hdf5_type<T>::value, type>::type
get_hdf5_type();

template<typename T>
inline typename std::enable_if<is_hdf5_type<T>::value &&
                               !is_predefined_hdf5_type<T>::value, type>::type
get_hdf5_type();

template<typename T>
inline typename std::enable_if<!is_hdf5_type<T>::value, type>::type
get_hdf5_type();

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


namespace detail
{

template<typename T,std::size_t I,typename Enable = void>
struct add_member_to_layout;

template<typename T,std::size_t I>
struct add_member_to_layout<T,I,
             typename std::enable_if<static_type_layout<T>::category ==
                                     static_type_layout_category::generic>::type>
{
    static std::size_t eval(type_layout& layout,std::size_t current_offset)
    {
        const id_offset_pair id_offset = static_type_layout<T>::members()[I];

        typedef typename std::tuple_element<
                             I,
                             typename static_type_layout<T>::member_types
                           >::type
                member_type;

        layout.add_element(id_offset.id,
                           get_hdf5_type<member_type>(),
                           id_offset.offset);

        return id_offset.offset;
    }
};

template<typename T,std::size_t I>
struct add_member_to_layout<T,I,
             typename std::enable_if<static_type_layout<T>::category ==
                                     static_type_layout_category::packed>::type>
{
    static std::size_t eval(type_layout& layout,std::size_t current_offset)
    {
        const id_size_pair id_size = static_type_layout<T>::members()[I];

        typedef typename std::tuple_element<
                             I,
                             typename static_type_layout<T>::member_types
                           >::type
                member_type;

        layout.add_element(id_size.id,
                           get_hdf5_type<member_type>(),
                           current_offset);

        return current_offset + id_size.size;
    }
};

template<typename T,std::size_t I,std::size_t N,typename Enable = void>
struct add_members_to_layout;

template<typename T,std::size_t I,std::size_t N>
struct add_members_to_layout<T,I,N>
{
    static void eval(type_layout& layout,std::size_t current_offset)
    {
        const std::size_t new_offset =
           add_member_to_layout<T,I>::eval(layout,current_offset);

        add_members_to_layout<T,I+1,N>::eval(layout,new_offset);
    }
};

template<typename T,std::size_t N>
struct add_members_to_layout<T,N,N>
{
    static void eval(type_layout&,std::size_t)
    {
    }
};

}

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

    type_layout new_layout(static_type_layout<value_type>::size);

    constexpr std::size_t num_members = static_type_layout<value_type>::num_members;

    detail::add_members_to_layout<value_type,0,num_members>::eval(new_layout,0);

    return type::compound_type(new_layout);
}

template<typename T>
inline typename std::enable_if<!is_hdf5_type<T>::value, type>::type
get_hdf5_type()
{
    static_assert(is_hdf5_type<T>::value,"T must be a valid HDF5 type");
}
}

#endif
