#ifndef ECHELON_CUSTOMIZATION_HOOKS_HPP
#define ECHELON_CUSTOMIZATION_HOOKS_HPP

#include <type_traits>

#include <string>
#include <vector>

#include <cassert>

namespace echelon
{

template<typename T>
struct dataset_read_hook
{
    static const bool is_specialized = false;
};

template<typename T>
struct dataset_read_hook<std::vector<T>>
{
    static const bool is_specialized = true;

    static std::vector<T> create(const std::vector<std::size_t>& dims)
    {
        assert(dims.size() == 1);

        return std::vector<T>(dims[0]);
    }

    static auto data(std::vector<T>& v) -> decltype(v.data())
    {
        return v.data();
    }
};

template<typename T>
struct dataset_write_hook
{
    static const bool is_specialized = false;
};

template<typename T>
struct dataset_write_hook<std::vector<T>>
{
    static const bool is_specialized = true;

    static std::vector<std::size_t> dims(const std::vector<T>& v)
    {
        return { v.size() };
    }

    static auto data(const std::vector<T>& v) -> decltype(v.data())
    {
        return v.data();
    }
};

template<typename T>
struct type_lowering_hook
{
    typedef typename std::decay<T>::type original_type;
    typedef typename std::decay<T>::type lowered_type;

    static lowered_type lower_type(original_type value)
    {
        return value;
    }

    static original_type raise_type(lowered_type value)
    {
        return value;
    }
};

template<>
struct type_lowering_hook<std::string>
{
    typedef std::string original_type;
    typedef const char* lowered_type;

    static lowered_type lower_type(const original_type& value)
    {
        return value.c_str();
    }

    static original_type raise_type(lowered_type value)
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
