#ifndef ECHELON_MULTI_ARRAY_ADAPTOR_HPP
#define ECHELON_MULTI_ARRAY_ADAPTOR_HPP

#include <echelon/container_adaption.hpp>
#include <echelon/dataset.hpp>
#include <echelon/detail/map_indices.hpp>
#include <echelon/detail/all_integral.hpp>

#include <algorithm>
#include <functional>
#include <vector>

namespace echelon
{

template<typename Container>
class multi_array_adaptor
{
public:
    typedef typename Container::value_type value_type;

    multi_array_adaptor(Container& container_,
                        const std::vector<std::size_t>& shape_)
    :container_(container_),shape_(shape_)
    {}

    template<typename... Indices>
    const value_type& operator()(Indices... indices)const
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return container_[detail::map_indices(shape_,indices...)];
    }

    template<typename... Indices>
    value_type& operator()(Indices... indices)
    {
        static_assert(detail::all_integral<Indices...>::value,
                      "All indices must be of integral type.");

        return container_[detail::map_indices(shape_,indices...)];
    }

    typename Container::iterator begin()
    {
        return container_.begin();
    }

    typename Container::iterator end()
    {
        return container_.end();
    }

    typename Container::const_iterator begin()const
    {
        return container_.begin();
    }

    typename Container::const_iterator end()const
    {
        return container_.end();
    }

    const value_type* data()const
    {
        using ::echelon::data;

        return data(container_);
    }

    value_type* data()
    {
        using ::echelon::data;

        return data(container_);
    }

    const std::vector<std::size_t>& shape()const
    {
        return shape_;
    }
private:
    Container& container_;
    std::vector<std::size_t> shape_;
};

}

#endif
