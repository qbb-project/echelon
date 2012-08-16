#ifndef ECHELON_CUSTOMIZATION_HOOKS_HPP
#define ECHELON_CUSTOMIZATION_HOOKS_HPP

#include <type_traits>

#include <string>
#include <vector>

#include <cassert>

namespace echelon
{

template<typename T>
struct type_lowering_hook
{
    typedef typename std::decay<T>::type original_type;
    typedef typename std::decay<T>::type lowered_type;

    template<typename Sink>
    static lowered_type lower_type(original_type value,const Sink& sink)
    {
        return value;
    }

    template<typename Source>
    static original_type raise_type(lowered_type value,const Source& source)
    {
        return value;
    }
};

template<>
struct type_lowering_hook<std::string>
{
    typedef std::string original_type;
    typedef const char* lowered_type;

    template<typename Sink>
    static lowered_type lower_type(const original_type& value,const Sink& sink)
    {
        return value.c_str();
    }

    template<typename Source>
    static original_type raise_type(lowered_type value,const Source& source)
    {
        return std::string(value);
    }
};

template<typename T>
struct is_identity_lowering_function
{
    using original_type = typename T::original_type;

    using lowered_type = typename T::lowered_type;

    static const bool value = std::is_same<
                                 original_type,
                                 lowered_type
                               >::value;
};

}

#endif
