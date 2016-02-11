//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_CONTAINER_ADAPTION_HPP
#define ECHELON_HDF5_CONTAINER_ADAPTION_HPP

#include <utility>
#include <type_traits>
#include <cstddef>
#include <vector>
#include <cassert>

namespace echelon
{
namespace hdf5
{

struct adl_enabler
{
};

// Note: Due to a bug in the Intel compiler, it is important that the overloads are
//       declared in this specific order.

template <typename C>
inline auto data(C&& container) -> decltype(data(container, adl_enabler{}))
{
    return data(container, adl_enabler{});
}

template <typename C>
inline auto data(C&& container) -> decltype(container.data())
{
    return container.data();
}

// Function template which simply forwards its arguments to an overload of
// shape.
// Its sole purpose is to ensure that the correct overload can be found by ADL.
template <typename C>
inline auto data_adl(C&& container) -> decltype(data(container))
{
    return data(container);
}

namespace detail
{
template <typename T>
constexpr auto has_user_provided_shape_property_impl(int)
    -> decltype((shape(std::declval<T>(), adl_enabler{}), bool{}))
{
    return true;
}

template <typename T>
constexpr bool has_user_provided_shape_property_impl(...)
{
    return false;
}

template <typename T>
constexpr bool has_user_provided_shape_property()
{
    return detail::has_user_provided_shape_property_impl<T>(0);
}
}

template <typename C>
inline auto shape(const C& container) ->
    typename std::enable_if<detail::has_user_provided_shape_property<C>(),
                            decltype(shape(container, adl_enabler{}))>::type
{
    return shape(container, adl_enabler{});
}

template <typename C>
inline auto shape(const C& container) ->
    typename std::enable_if<!detail::has_user_provided_shape_property<C>(),
                            decltype(container.shape())>::type
{
    return container.shape();
}

template <typename C>
inline auto shape_adl(const C& container) -> decltype(shape(container))
{
    return shape(container);
}

template <typename C>
inline auto reshape(C& container, const std::vector<std::size_t>& new_shape)
    -> decltype(reshape(container, new_shape, adl_enabler{}))
{
    return reshape(container, new_shape, adl_enabler{});
}

/** \brief Reshapes the container.
 *
 *  \tparam C type of the container
 *
 *  \param container container, which should be reshaped
 *  \param new_shape new shape of the container
 */
template <typename C>
inline auto reshape(C& container, const std::vector<std::size_t>& new_shape)
    -> decltype(container.reshape(new_shape))
{
    return container.reshape(new_shape);
}

template <typename C>
inline auto reshape_adl(C& container, const std::vector<std::size_t>& new_shape)
    -> decltype(reshape(container, new_shape))
{
    return reshape(container, new_shape);
}

template <typename C>
inline auto storage_order(C&& container) -> decltype(storage_order(container, adl_enabler{}))
{
    return storage_order(container, adl_enabler{});
}

template <typename Shape>
class row_major_storage_order
{
public:
    explicit row_major_storage_order(Shape shape_) : shape_(shape_)
    {
    }

    template <typename Indices>
    std::size_t map(const Indices& indices) const
    {
        std::size_t rank = shape_.size();

        assert(rank == indices.size());

        std::size_t address = 0;

        for (std::size_t i = 0; i < rank; ++i)
        {
            address = address * shape_[i] + indices[i];
        }

        return address;
    }

private:
    Shape shape_;
};

template<typename StorageOrder>
struct is_native_storage_order : std::false_type
{
};

template<typename Shape>
struct is_native_storage_order<row_major_storage_order<Shape>> : std::true_type
{
};

template <typename C>
inline auto storage_order(const C& container, adl_enabler)
-> decltype(row_major_storage_order<decltype(shape_adl(container))>(shape_adl(container)))
{
    return row_major_storage_order<decltype(shape_adl(container))>(shape_adl(container));
}

template <typename C>
inline auto storage_order_adl(C&& container) -> decltype(storage_order(container))
{
    return storage_order(container);
}

namespace detail
{

template <typename T>
constexpr auto has_data_accessor_impl(int) -> decltype((data_adl(std::declval<T>()), bool{}))
{
    return true;
}

template <typename T>
constexpr bool has_data_accessor_impl(...)
{
    return false;
}

template <typename T>
constexpr auto has_shape_property_impl(int) -> decltype((shape(std::declval<T>()), bool{}))
{
    return true;
}

template <typename T>
constexpr bool has_shape_property_impl(...)
{
    return false;
}

template <typename T>
constexpr auto has_reshape_member_impl(int)
    -> decltype((reshape(std::declval<T&>(), std::vector<std::size_t>{}), bool{}))
{
    return true;
}

template <typename T>
constexpr bool has_reshape_member_impl(...)
{
    return false;
}
}

template <typename T>
constexpr bool has_data_accessor()
{
    return detail::has_data_accessor_impl<T>(0);
}

template <typename T>
constexpr bool has_shape_property()
{
    return detail::has_shape_property_impl<T>(0);
}

template <typename T>
constexpr bool has_reshape_member()
{
    return detail::has_reshape_member_impl<T>(0);
}

template <typename T>
constexpr bool is_readable_container()
{
    return has_data_accessor<const T>() && has_shape_property<T>();
}

template <typename T>
constexpr bool is_container()
{
    return has_data_accessor<T>() && has_shape_property<T>();
}

template <typename C>
struct container_trait
{
    static_assert(is_container<C>(), "C does not fulfill the Container requirements.");

    using value_type = typename std::decay<decltype(*data_adl(std::declval<const C>()))>::type;
};
}
}

#endif
