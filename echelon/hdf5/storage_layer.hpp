//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_STORAGE_LAYER_HPP
#define ECHELON_HDF5_STORAGE_LAYER_HPP

#include <echelon/hdf5/customization_hooks.hpp>
#include <echelon/hdf5/type_traits.hpp>

#include <echelon/hdf5/container_adaption.hpp>

#include <echelon/hdf5/precursor/property_list.hpp>
#include <echelon/hdf5/precursor/dataspace.hpp>
#include <echelon/hdf5/precursor/type.hpp>

#include <utility>
#include <functional>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <type_traits>
#include <exception>

#include <boost/format.hpp>


namespace echelon
{
namespace hdf5
{

class inconsistent_selection_size_exception : public std::exception
{
public:
    explicit inconsistent_selection_size_exception(hssize_t mem_selection_size_, hssize_t file_selection_size_)
    {
        what_ = str(boost::format("The number of selected elements in the memory space (%1%) and the file space (%2%) differs.") % mem_selection_size_ % file_selection_size_);
    }

    const char* what() const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string what_;
};

template <typename Sink, typename C>
inline void write_impl(Sink& sink, const hdf5::precursor::type& datatype,
                       const hdf5::precursor::dataspace& memspace,
                       const hdf5::precursor::dataspace& filespace, const C& container,
                       std::true_type)
{
    static_assert(is_readable_container<C>(),
                  "C does not fulfill the ReadableContainer requirements");

    using value_type = typename std::decay<decltype(*data_adl(container))>::type;

    static_assert(is_hdf5_type<value_type>::value, "trivially storable types must be HDF5 types.");

    sink.write(datatype, memspace, filespace, hdf5::precursor::default_property_list,
               data_adl(container));
}

template <typename Sink, typename C>
void write(Sink& sink, const hdf5::precursor::type& datatype,
           const hdf5::precursor::dataspace& memspace, const hdf5::precursor::dataspace& filespace,
           const C& container);

template <typename Sink, typename C>
inline void write_impl(Sink& sink, const hdf5::precursor::type& datatype,
                       const hdf5::precursor::dataspace& memspace,
                       const hdf5::precursor::dataspace& filespace, const C& container,
                       std::false_type)
{
    static_assert(is_readable_container<C>(),
                  "C does not fulfill the ReadableContainer requirements");

    using lowered_type =
        typename std::decay<decltype(lower_type_internal(*data_adl(container), {}))>::type;

    std::size_t buffer_size = memspace.select_npoints();

    std::vector<lowered_type> lowered_buffer(buffer_size);

    for (std::size_t i = 0; i < buffer_size; ++i)
    {
        lowered_buffer[i] = lower_type_internal(data_adl(container)[i], sink);
    }

    write(sink, datatype, memspace, filespace, lowered_buffer);
}

template <typename Sink, typename C>
inline void write(Sink& sink, const hdf5::precursor::type& datatype,
                  const hdf5::precursor::dataspace& memspace,
                  const hdf5::precursor::dataspace& filespace, const C& container)
{
    static_assert(is_readable_container<C>(),
                  "C does not fulfill the ReadableContainer requirements");

    using value_type = typename std::decay<decltype(*data_adl(container))>::type;

    write_impl(sink, datatype, memspace, filespace, container,
               std::integral_constant<bool, is_trivially_storable<value_type>()>{});
}

template <typename Source, typename C>
void read(const Source& source, const hdf5::precursor::type& datatype,
          const hdf5::precursor::dataspace& memspace, const hdf5::precursor::dataspace& filespace,
          C& container);

template <typename Source, typename C>
inline void read_impl(const Source& source, const hdf5::precursor::type& datatype,
                      const hdf5::precursor::dataspace& memspace,
                      const hdf5::precursor::dataspace& filespace, C& container, std::true_type)
{
    static_assert(is_container<C>(), "C does not fulfill the Container requirements");

    using value_type = typename std::decay<decltype(*data_adl(container))>::type;

    static_assert(is_hdf5_type<value_type>::value, "trivially storable types must be HDF5 types.");

    auto memory_selection_size = memspace.select_npoints();
    auto file_selection_size = filespace.select_npoints(); 
    
    if(memory_selection_size != file_selection_size)
        throw inconsistent_selection_size_exception(memory_selection_size, file_selection_size);

    source.read(datatype, memspace, filespace, hdf5::precursor::default_property_list,
                data_adl(container));
}

template <typename Source, typename C>
inline void read_impl(const Source& source, const hdf5::precursor::type& datatype,
                      const hdf5::precursor::dataspace& memspace,
                      const hdf5::precursor::dataspace& filespace, C& container, std::false_type)
{
    static_assert(is_container<C>(), "C does not fulfill the Container requirements");

    std::vector<hsize_t> mem_shape = memspace.get_simple_extent_dims();

    using lowered_type =
        typename std::decay<decltype(lower_type_internal(*data_adl(container), {}))>::type;

    auto memory_selection_size = memspace.select_npoints();
    auto file_selection_size = filespace.select_npoints(); 

    if(memory_selection_size != file_selection_size)
        throw inconsistent_selection_size_exception(memory_selection_size, file_selection_size);
    
    std::vector<lowered_type> lowered_buffer(memory_selection_size);

    read(source, datatype, memspace, filespace, lowered_buffer);

    for (decltype(memory_selection_size) i = 0; i < memory_selection_size; ++i)
    {
        data_adl(container)[i] = raise_type_internal(lowered_buffer[i], source);
    }

    if (H5Tis_variable_str(datatype.id()) || H5Tget_class(datatype.id()) == H5T_VLEN)
    {
        H5Dvlen_reclaim(datatype.id(), memspace.id(), H5P_DEFAULT, lowered_buffer.data());
    }
}

template <typename Source, typename C>
inline void read(const Source& source, const hdf5::precursor::type& datatype,
                 const hdf5::precursor::dataspace& memspace,
                 const hdf5::precursor::dataspace& filespace, C& container)
{
    static_assert(is_container<C>(), "C does not fulfill the Container requirements");

    using value_type = typename std::decay<decltype(*data_adl(container))>::type;

    read_impl(source, datatype, memspace, filespace, container,
              std::integral_constant<bool, is_trivially_storable<value_type>()>{});
}

template <typename Sink, typename T>
void write(Sink& sink, const T& value);

template <typename Sink, typename T>
inline void write_impl(Sink& sink, const T& value, std::true_type)
{
    static_assert(is_hdf5_type<T>::value, "trivially storable types must be HDF5 types.");

    sink.write(&value);
}

template <typename Sink, typename T>
inline void write_impl(Sink& sink, const T& value, std::false_type)
{
    auto lowered_value = lower_type_internal(value, sink);

    write(sink, lowered_value);
}

template <typename Sink, typename T>
inline void write(Sink& sink, const T& value)
{
    write_impl(sink, value, std::integral_constant<bool, is_trivially_storable<T>()>{});
}

template <typename Sink, typename T, int N>
inline void write(Sink& sink, const T (&value)[N])
{
    using value_type = typename std::decay<T>::type;

    static_assert(is_trivially_storable<value_type>(),
                  "Storage of arrays of non-trivially storable types is not implemented.");

    const T* decayed_array = value;

    sink.write(&decayed_array);
}

template <typename Source, typename T>
inline void read(Source& source, T& value);

template <typename Source, typename T>
inline void read_impl(Source& source, T& value, std::true_type)
{
    static_assert(is_hdf5_type<T>::value, "trivially storable types must be HDF5 types.");

    source.read(&value);
}

template <typename Source, typename T>
inline void read_impl(Source& source, T& value, std::false_type)
{
    using lowered_value_type = typename std::decay<decltype(lower_type_internal(value, {}))>::type;

    lowered_value_type lowered_value;

    read(source, lowered_value);

    value = raise_type_internal(lowered_value, source);

    auto datatype = source.datatype();

    if (H5Tis_variable_str(datatype.id()) || H5Tget_class(datatype.id()) == H5T_VLEN)
    {

        H5Dvlen_reclaim(datatype.id(), hdf5::precursor::dataspace().id(), H5P_DEFAULT,
                        &lowered_value);
    }
}

template <typename Source, typename T>
inline void read(Source& source, T& value)
{
    using value_type = typename std::decay<T>::type;

    read_impl(source, value, std::integral_constant<bool, is_trivially_storable<value_type>()>{});
}

template <typename Source, typename T, int N>
inline void read(Source& source, T (&value)[N])
{
    using value_type = typename std::decay<T>::type;

    static_assert(is_trivially_storable<value_type>(),
                  "Storage of arrays of non-trivially storable types is not implemented.");

    T* decayed_array = value;

    source.read(&decayed_array);
}
}
}

#endif
