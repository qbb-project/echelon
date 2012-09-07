#ifndef ECHELON_MULTI_ARRAY_HPP
#define ECHELON_MULTI_ARRAY_HPP

#include <vector>
#include <algorithm>
#include <functional>

#include <echelon/customization_hooks.hpp>

namespace echelon
{

template<typename T>
class multi_array
{
public:
    typedef T value_type;

    multi_array()=default;

    explicit multi_array(const std::vector<std::size_t>& shape_)
    :data_(std::accumulate(std::begin(shape_),std::end(shape_),
                           std::size_t(1),std::multiplies<std::size_t>())),
     shape_(shape_)
    {}

    const T& operator()(std::size_t i)const
    {
        return data_[i];
    }

    T& operator()(std::size_t i)
    {
        return data_[i];
    }

    const T& operator()(std::size_t i,std::size_t j)const
    {
        return data_[shape_[1]*i + j];
    }

    T& operator()(std::size_t i,std::size_t j)
    {
        return data_[shape_[1]*i + j];
    }

    const T* data()const
    {
        return data_.data();
    }

    T* data()
    {
        return data_.data();
    }

    typename std::vector<T>::iterator begin()
    {
        return data_.begin();
    }

    typename std::vector<T>::iterator end()
    {
        return data_.end();
    }

    typename std::vector<T>::const_iterator begin()const
    {
        return data_.begin();
    }

    typename std::vector<T>::const_iterator end()const
    {
        return data_.end();
    }

    const std::vector<std::size_t>& shape()const
    {
        return shape_;
    }

    void resize(const std::vector<std::size_t>& dims_)
    {
        data_.resize(std::accumulate(std::begin(dims_),std::end(dims_),
                                     std::size_t(1),std::multiplies<std::size_t>()));

        this->shape_ = dims_;
    }
private:
    std::vector<T> data_;
    std::vector<std::size_t> shape_;
};

template<typename T>
inline void require_dimensions(multi_array<T>& container,
                               const std::vector<std::size_t>& dims)
{
    container.resize(dims);
}

}

#endif
