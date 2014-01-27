//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_CUSTOMIZATION_HOOKS_HPP
#define ECHELON_HDF5_CUSTOMIZATION_HOOKS_HPP

#include <type_traits>

#include <string>
#include <vector>

#include <cassert>

namespace echelon
{
namespace hdf5
{
template <typename T>
struct type_lowering_hook
{
    typedef typename std::decay<T>::type original_type;
    typedef typename std::decay<T>::type lowered_type;

    template <typename Sink>
    static lowered_type lower_type(original_type value, const Sink&)
    {
        return value;
    }

    template <typename Source>
    static original_type raise_type(lowered_type value, const Source&)
    {
        return value;
    }
};

template <>
struct type_lowering_hook<std::string>
{
    typedef std::string original_type;
    typedef const char* lowered_type;

    template <typename Sink>
    static lowered_type lower_type(const original_type& value, const Sink&)
    {
        return value.c_str();
    }

    template <typename Source>
    static original_type raise_type(lowered_type value, const Source&)
    {
        return std::string(value);
    }
};

template <typename T>
struct is_identity_lowering_function
{
    typedef typename T::original_type original_type;

    typedef typename T::lowered_type lowered_type;

    static const bool value = std::is_same<original_type, lowered_type>::value;
};
}
}

#endif
