#ifndef ECHELON_MULTI_ARRAY_ADAPTOR_HPP
#define ECHELON_MULTI_ARRAY_ADAPTOR_HPP

#include <algorithm>
#include <functional>
#include <vector>

#include <echelon/customization_hooks.hpp>

#include <echelon/dataset.hpp>

namespace echelon
{

template<typename Container>
inline auto data(const Container& container) -> decltype(container.data())
{
    return container.data();
}

template<typename Container>
inline auto data(Container& container) -> decltype(container.data())
{
    return container.data();
}

template<typename Container>
class multi_array_adaptor
{
public:
    using value_type = typename Container::value_type;

    multi_array_adaptor(Container& container_,
                        const std::vector<std::size_t>& dims_)
    :container_(container_),dims_(dims_)
    {}

    multi_array_adaptor& operator=(const dataset& dset)
    {
        std::vector<hsize_t> h5_dims(begin(dims()),end(dims()));

        dset.read(data(),h5_dims);

        return *this;
    }

    const value_type& operator()(std::size_t i,std::size_t j)const
    {
        return container_[dims_[1]*i + j];
    }

    value_type& operator()(std::size_t i,std::size_t j)
    {
        return container_[dims_[1]*i + j];
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

template<typename T>
struct dataset_write_hook<multi_array_adaptor<T> >
{
    static const bool is_specialized = true;

    static std::vector<std::size_t> dims(const multi_array_adaptor<T>& v)
    {
        return v.dims();
    }

    static auto data(const multi_array_adaptor<T>& v) -> decltype(v.data())
    {
        return v.data();
    }
};

}

#endif
