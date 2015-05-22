//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_RANGE_HPP
#define ECHELON_HDF5_RANGE_HPP

#include <echelon/utility/macros.hpp>

#include <hdf5.h>
#include <type_traits>

namespace echelon
{
namespace hdf5
{
template <typename Base, typename Bound>
class range_t
{
public:
    range_t(Base base_, Bound bound_, hsize_t stride_ = 1)
    : base_(base_), bound_(bound_), stride_(stride_)
    {
    }

    template <typename OtherBase, typename OtherBound,
              typename Dummy = typename std::enable_if<std::is_convertible<
                  Base, OtherBase>::value&& std::is_convertible<Bound, OtherBound>::value>::type>
    range_t(range_t<OtherBase, OtherBound> other)
    : base_(other.base()), bound_(other.bound()), stride_(other.stride())
    {
    }

    Base base() const
    {
        return base_;
    }

    Bound bound() const
    {
        return bound_;
    }

    hsize_t stride() const
    {
        return stride_;
    }

private:
    Base base_;
    Bound bound_;
    hsize_t stride_;
};

/** \brief Constructs an index range.
 *
 *  \param base base index (or lower bound)
 *  \param bound upper bound
 *  \param stride difference between two consecutive indices
 */
template <typename Base, typename Bound>
inline range_t<Base, Bound> range(Base base, Bound bound, hsize_t stride = 1)
{
    return range_t<Base, Bound>{base, bound, stride};
}

typedef range_t<hsize_t, hsize_t> totally_bound_range_t;

struct unbound_t
{
};

static const unbound_t _ = {};

namespace detail
{

template <std::size_t I, typename... Args>
struct calculate_slice_boundaries;

template <std::size_t I>
struct calculate_slice_boundaries<I>
{
    static void eval(const std::vector<hsize_t>&, std::vector<totally_bound_range_t>&)
    {
    }
};

template <std::size_t I, typename Front, typename... Tail>
struct calculate_slice_boundaries<I, Front, Tail...>
{
    static void eval(const std::vector<hsize_t>& current_shape,
                     std::vector<totally_bound_range_t>& boundaries, Front front, Tail... tail)
    {
        boundaries.push_back(get_boundaries(current_shape[I], front));

        calculate_slice_boundaries<I + 1, Tail...>::eval(current_shape, boundaries, tail...);
    }

private:
    static totally_bound_range_t get_boundaries(hsize_t extend, unbound_t)
    {
        return range(0, extend);
    }

    template <typename Base>
    static totally_bound_range_t get_boundaries(hsize_t extend, range_t<Base, unbound_t> r)
    {
        // FIXME: add a bound check

        return range(r.base(), extend);
    }

    template <typename Bound>
    static totally_bound_range_t get_boundaries(hsize_t, range_t<unbound_t, Bound> r)
    {
        // FIXME: add a bound check

        return range(0, r.bound());
    }

    template <typename Base, typename Bound>
    static totally_bound_range_t get_boundaries(hsize_t, range_t<Base, Bound> r)
    {
        static_assert(std::is_integral<Base>::value && std::is_integral<Bound>::value,
                      "only integral values are allowed in slicing expressions");

        // FIXME: add a bound check

        return r;
    }

    template <typename T>
    static totally_bound_range_t get_boundaries(hsize_t ECHELON_UNUSED_RELEASE(extend), T value)
    {
        static_assert(std::is_integral<T>::value,
                      "only integral values are allowed in slicing expressions");

        assert(value < extend);

        return range(value, value + 1);
    }
};
}



}
}

#endif
