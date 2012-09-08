#ifndef ECHELON_DETAIL_ALL_INTEGRAL_HPP
#define ECHELON_DETAIL_ALL_INTEGRAL_HPP

#include <type_traits>

namespace echelon
{
namespace detail
{

template<typename... T>
struct and_;

template<typename Front>
struct and_<Front> : std::integral_constant<bool, Front::value>
{
};

template<typename Front,typename... Tail>
struct and_<Front,Tail...> : std::integral_constant<bool, Front::value && and_<Tail...>::value>
{
};

template<typename... T>
struct all_integral:
    and_<std::is_integral<typename std::remove_reference<T>::type >...>
{};

}
}

#endif
