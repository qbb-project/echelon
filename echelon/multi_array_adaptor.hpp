#ifndef ECHELON_MULTI_ARRAY_ADAPTOR_HPP
#define ECHELON_MULTI_ARRAY_ADAPTOR_HPP

#include <algorithm>
#include <functional>
#include <vector>

#include <echelon/container_adaption.hpp>

#include <echelon/dataset.hpp>

namespace echelon
{

template<typename Container>
class multi_array_adaptor
{
public:
    typedef typename Container::value_type value_type;

    multi_array_adaptor(Container& container_,
                        const std::vector<std::size_t>& dims_)
    :container_(container_),dims_(dims_)
    {}

    const value_type& operator()(std::size_t i,std::size_t j)const
    {
        return container_[dims_[1]*i + j];
    }

    value_type& operator()(std::size_t i,std::size_t j)
    {
        return container_[dims_[1]*i + j];
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

    const std::vector<std::size_t>& dims()const
    {
        return dims_;
    }
private:
    Container& container_;
    std::vector<std::size_t> dims_;
};

}

#endif
