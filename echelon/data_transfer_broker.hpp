//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_DATA_TRANSFER_BROKER_HPP
#define ECHELON_DATA_TRANSFER_BROKER_HPP

#include <echelon/customization_hooks.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/type.hpp>
#include <echelon/object_reference.hpp>

#include <echelon/container_adaption.hpp>

#include <utility>
#include <functional>
#include <vector>
#include <iterator>
#include <algorithm>

#include <boost/iterator/transform_iterator.hpp>

namespace echelon
{

template<typename T>
struct remove_base_type_cv
{
    typedef typename std::remove_volatile<typename std::remove_const<T>::type>::type type;
};

template<typename T>
struct remove_base_type_cv<T*>
{
    typedef typename std::remove_volatile<typename std::remove_const<T>::type>::type* type;
};

template<typename Sink,typename C>
inline
typename std::enable_if<is_identity_lowering_function<type_lowering_hook<typename C::value_type> >::value >::type
write(Sink& sink,const hdf5::type& datatype,
      const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
      const C& container)
{
    sink.write(datatype, memspace, filespace,
               hdf5::default_property_list,
               data(container));
}

template<typename Sink,typename C>
inline
typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<typename C::value_type> >::value >::type
write(Sink& sink,const hdf5::type& datatype,
      const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
      const C& container)
{
    typedef typename C::value_type T;

    typedef typename type_lowering_hook<T>::lowered_type lowered_type;

    std::vector<lowered_type> lowered_data;

    for(const T& value : container)
    {
        lowered_data.push_back(type_lowering_hook<T>::lower_type(value,sink));
    }

    write(sink,datatype,memspace,filespace,lowered_data);
}

template<typename Source,typename C>
inline
typename std::enable_if<is_identity_lowering_function<type_lowering_hook<typename C::value_type> >::value >::type
read(const Source& source,const hdf5::type& datatype,
     const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
     C& container)
{
    std::vector<hsize_t> mem_shape = memspace.get_simple_extent_dims();
    std::vector<std::size_t> mem_shape_(begin(mem_shape),end(mem_shape));

    reshape(container,mem_shape_);

    source.read(datatype,memspace,filespace,
                hdf5::default_property_list,data(container));
}

template<typename Source,typename C>
inline
typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<typename C::value_type> >::value >::type
read(const Source& source,const hdf5::type& datatype,
     const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
     C& container)
{
    typedef typename C::value_type T;

    typedef typename remove_base_type_cv<
                   typename type_lowering_hook<T>::lowered_type
                  >::type lowered_type;

    std::vector<lowered_type> lowered_data;

    read(source,datatype,memspace,filespace,lowered_data);

    auto raise = std::bind(type_lowering_hook<T>::template raise_type<Source>,
                           std::placeholders::_1,
                           std::cref(source));

    fill(container,boost::make_transform_iterator(begin(lowered_data),raise),
                   boost::make_transform_iterator(end(lowered_data),raise));

    if( H5Tis_variable_str(datatype.id()) ||
        H5Tget_class(datatype.id()) == H5T_VLEN)
    {

        H5Dvlen_reclaim(datatype.id(),memspace.id(),
                        H5P_DEFAULT,lowered_data.data());
    }
}

template<typename Sink,
         typename T,
         typename std::enable_if<is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void write(Sink& sink,const T& value)
{
    sink.write(&value);
}

//enforce array-to-pointer decay
template<typename Sink,
         typename T,
         std::size_t N,
         typename std::enable_if<is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void write(Sink& sink,const T (&value) [N])
{
    const T* decayed_array = value;
    sink.write(&decayed_array);
}

template<typename Sink,
         typename T,
         typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void write(Sink& sink,const T& value)
{
    write(sink,type_lowering_hook<T>::lower_type(value,sink));
}

template<typename Source,
         typename T,
         typename std::enable_if<is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void read(Source& source,T& value)
{
    source.read(&value);
}

//enforce array-to-pointer decay
template<typename Source,
         typename T,
         std::size_t N,
         typename std::enable_if<is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void read(Source& source,T (&value) [N])
{
    T* decayed_array = value;
    source.read(&decayed_array);
}

template<typename Source,
         typename T,
         typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void read(Source& source,T& value)
{
    typedef typename remove_base_type_cv<
                   typename type_lowering_hook<T>::lowered_type
                  >::type lowered_type;

    lowered_type lowered_value;

    read(source,lowered_value);

    value = type_lowering_hook<T>::raise_type(lowered_value,source);

    auto datatype = source.datatype();

    if( H5Tis_variable_str(datatype.id()) ||
        H5Tget_class(datatype.id()) == H5T_VLEN)
    {

        H5Dvlen_reclaim(datatype.id(),hdf5::dataspace().id(),
                        H5P_DEFAULT,&lowered_value);
    }
}


}

#endif
