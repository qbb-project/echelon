#ifndef ECHELON_DATA_TRANSFER_BROKER_HPP
#define ECHELON_DATA_TRANSFER_BROKER_HPP

#include <echelon/customization_hooks.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/type.hpp>
#include <echelon/object_reference.hpp>

#include <utility>
#include <functional>
#include <vector>
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

template<typename Sink,typename T>
inline
typename std::enable_if<is_identity_lowering_function<type_lowering_hook<T> >::value >::type
write(Sink& sink,const hdf5::type& datatype,
      const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
      const T* data)
{
    sink.write(datatype, memspace, filespace,
               hdf5::default_property_list,
               data);
}

template<typename Sink>
inline void write(Sink& sink,const hdf5::type& datatype,
                  const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
                  const object_reference* data)
{
    const hssize_t size = memspace.select_npoints();

    std::vector<hdf5::object_reference> raw_refs(size);

    std::transform(data,data + size,begin(raw_refs),
                   [](const object_reference& ref) -> hdf5::object_reference
                   {
                      return ref.raw_ref();
                   });

    sink.write(datatype, memspace, filespace,
               hdf5::default_property_list,
               raw_refs.data());
}

template<typename Sink,typename T>
inline
typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<T> >::value >::type
write(Sink& sink,const hdf5::type& datatype,
      const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
      const T* data)
{
    using lowered_type = typename type_lowering_hook<T>::lowered_type;

    const hssize_t size = memspace.select_npoints();

    std::vector<lowered_type> lowered_data(
              boost::make_transform_iterator(data,type_lowering_hook<T>::lower_type),
              boost::make_transform_iterator(data + size,type_lowering_hook<T>::lower_type));

    write(sink,datatype,memspace,filespace,lowered_data.data());
}

template<typename Source,typename T>
inline
typename std::enable_if<is_identity_lowering_function<type_lowering_hook<T> >::value >::type
read(const Source& source,const hdf5::type& datatype,
     const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
     T* data)
{
    source.read(datatype,memspace,filespace,
                hdf5::default_property_list,data);
}

template<typename Source>
inline void read(const Source& source,const hdf5::type& datatype,
                 const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
                 object_reference* data)
{
    const hssize_t size = memspace.select_npoints();

    std::vector<hdf5::object_reference> raw_refs(size);

    source.read(datatype,memspace,filespace,
                hdf5::default_property_list,raw_refs.data());

    object any_object(source.id());

    std::transform(begin(raw_refs),end(raw_refs),data,
                   [any_object](const hdf5::object_reference& raw_ref) -> object_reference
                   {
                      return object_reference(raw_ref,any_object);
                   });
}

template<typename Source,typename T>
inline
typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<T> >::value >::type
read(const Source& source,const hdf5::type& datatype,
     const hdf5::dataspace& memspace,const hdf5::dataspace& filespace,
     T* data)
{
    using lowered_type = typename remove_base_type_cv<
                            typename type_lowering_hook<T>::lowered_type
                          >::type;

    const hssize_t size = memspace.select_npoints();

    std::vector<lowered_type> lowered_data(size);

    read(source,datatype,memspace,filespace,lowered_data.data());

    std::transform(begin(lowered_data),end(lowered_data),
                   data,type_lowering_hook<T>::raise_type);

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

template<typename Sink>
inline void write(Sink& sink,const object_reference& value)
{
    hdf5::object_reference raw_ref = value.raw_ref();
    sink.write(&raw_ref);
}

template<typename Sink,
         typename T,
         typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void write(Sink& sink,const T& value)
{
    write(sink,type_lowering_hook<T>::lower_type(value));
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

template<typename Source>
inline void write(Source& source,object_reference& value)
{
    hdf5::object_reference raw_ref;

    source.read(&raw_ref);

    value = object_reference(raw_ref,object(source.id()));
}

template<typename Source,
         typename T,
         typename std::enable_if<!is_identity_lowering_function<type_lowering_hook<T> >::value,
                                 int>::type dummy = 0>
inline void read(Source& source,T& value)
{
    using lowered_type = typename remove_base_type_cv<
                           typename type_lowering_hook<T>::lowered_type
                          >::type;

    lowered_type lowered_value;

    read(source,lowered_value);

    value = type_lowering_hook<T>::raise_type(lowered_value);
}


}

#endif
