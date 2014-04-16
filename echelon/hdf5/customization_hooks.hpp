//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_CUSTOMIZATION_HOOKS_HPP
#define ECHELON_HDF5_CUSTOMIZATION_HOOKS_HPP

// FIXME: enable raise_type to be called via ADL and remove this include
#include <echelon/object_reference.hpp>

#include <echelon/hdf5/object_reference.hpp>
#include <echelon/hdf5/precursor/object_reference.hpp>
#include <echelon/hdf5/precursor/handle.hpp>

#include <type_traits>
#include <utility>
#include <string>
#include <vector>

namespace echelon
{
namespace hdf5
{

template <typename T>
class coersion_protected_ref
{
public:
    coersion_protected_ref(T& value_) : value_{&value_}
    {
    }

    operator T&()
    {
        return *value_;
    }

    T& get()
    {
        return *value_;
    }

private:
    T* value_;
};

inline const char* lower_type(coersion_protected_ref<const std::string> value)
{
    return value.get().c_str();
}

inline std::string raise_type(const char* value)
{
    return std::string(value);
}

inline hdf5::object_reference lower_type(const echelon::object_reference& value)
{
    return value.raw_ref();
}

inline echelon::object_reference raise_type(const hdf5::object_reference& value)
{
    return echelon::object_reference(value);
}

template <typename T>
inline auto lower_type_internal(T&& value, precursor::handle)
    -> decltype(lower_type(std::forward<T>(value)))
{
    return lower_type(std::forward<T>(value));
}

template <typename T>
inline auto raise_type_internal(T&& value, precursor::handle)
    -> decltype(raise_type(std::forward<T>(value)))
{
    return raise_type(std::forward<T>(value));
}

inline precursor::object_reference lower_type_internal(const object_reference& value,
                                                       precursor::handle)
{
    return value.raw_ref();
}

inline object_reference raise_type_internal(const precursor::object_reference& value,
                                            precursor::handle internal_handle)
{
    return object_reference(value, internal_handle);
}

namespace detail
{

template <typename T>
constexpr auto is_trivially_storable_impl(int)
    -> decltype((lower_type_internal(std::declval<T>(), std::declval<precursor::handle>()),
                 bool {}))
{
    return false;
}

template <typename T>
constexpr bool is_trivially_storable_impl(...)
{
    return true;
}
}

template <typename T>
constexpr bool is_trivially_storable()
{
    return detail::is_trivially_storable_impl<T>(0);
}

static_assert(!is_trivially_storable<std::string>(),
              "std::string should not be trivially storable.");
}
}

#endif
