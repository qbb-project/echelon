#ifndef ECHELON_CONTAINER_ADAPTION_HPP
#define ECHELON_CONTAINER_ADAPTION_HPP

#include <echelon/std/stl_support.hpp>

namespace echelon
{

template<typename C>
inline auto data(const C& container) -> decltype(container.data())
{
    return container.data();
}

template<typename C>
inline auto data(C& container) -> decltype(container.data())
{
    return container.data();
}

}

#endif
