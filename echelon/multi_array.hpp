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
    explicit multi_array(const std::vector<std::size_t>& dims_)
    :data_(std::accumulate(begin(dims_),end(dims_),
                           std::size_t(1),std::multiplies<std::size_t>())),
     dims_(dims_)
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
        return data_[dims_[1]*i + j];
    }

    T& operator()(std::size_t i,std::size_t j)
    {
        return data_[dims_[1]*i + j];
    }

    const T* data()const
    {
        return data_.data();
    }

    T* data()
    {
        return data_.data();
    }

    const std::vector<std::size_t>& dims()const
    {
        return dims_;
    }
private:
    std::vector<T> data_;
    std::vector<std::size_t> dims_;
};

template<typename T>
struct dataset_write_hook<multi_array<T> >
{
    static const bool is_specialized = true;

    static std::vector<std::size_t> dims(const multi_array<T>& v)
    {
        return v.dims();
    }

    static auto data(const multi_array<T>& v) -> decltype(v.data())
    {
        return v.data();
    }
};

template<typename T>
struct dataset_read_hook<multi_array<T> >
{
    static const bool is_specialized = true;

    static multi_array<T> create(const std::vector<std::size_t>& dims)
    {
        return multi_array<T>(dims);
    }

    static auto data(multi_array<T>& v) -> decltype(v.data())
    {
        return v.data();
    }
};

}

#endif
