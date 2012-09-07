#ifndef ECHELON_DATASET_HPP
#define ECHELON_DATASET_HPP

#include <echelon/object.hpp>

#include <echelon/type.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/property_list.hpp>
#include <echelon/type_factory.hpp>
#include <echelon/object_reference.hpp>

#include <echelon/slice.hpp>
#include <echelon/range.hpp>

#include <echelon/attribute_repository.hpp>
#include <echelon/data_transfer_broker.hpp>

#include <vector>
#include <tuple>
#include <string>
#include <type_traits>
#include <cassert>


namespace echelon
{

struct unbound_t {};

static const unbound_t _ = {};


namespace detail
{

template<std::size_t I,typename ...Args>
struct calculate_slice_boundaries;

template<std::size_t I>
struct calculate_slice_boundaries<I>
{
    static void eval(const std::vector<hsize_t>&,
                     std::vector<range>&)
    {
    }
};

template<std::size_t I,typename Front,typename ...Tail>
struct calculate_slice_boundaries<I,Front,Tail...>
{
    static void eval(const std::vector<hsize_t>& current_shape,
                     std::vector<range>& boundaries,
                     Front front,
                     Tail... tail)
    {
        boundaries.push_back(get_boundaries(current_shape[I],front));

        calculate_slice_boundaries<I+1,Tail...>::eval(current_shape,boundaries,tail...);
    }
private:
    static range get_boundaries(hsize_t extend,unbound_t)
    {
        return range(0,extend);
    }

    static range get_boundaries(hsize_t,range r)
    {
        return r;
    }

    template<typename T>
    static range get_boundaries(hsize_t extend,T value)
    {
        static_assert(std::is_integral<T>::value,
                      "only integral values are allowed in slicing expressions");

        assert(value < extend);

        return range(value,value + 1);
    }
};

}

class dataset
{
public:
    dataset(const object& parent, const std::string& name,
            const type& datatype, const std::vector<hsize_t>& dims,
            int comp_level = -1);

    explicit dataset(hdf5::dataset dataset_wrapper_);

    template<typename T>
    friend void operator<<=(dataset& ds,const T& array)
    {
        auto current_shape = detail::shape_adl(array);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::dataspace mem_space(mem_shape);
        hdf5::dataspace file_space = ds.dataset_wrapper_.get_space();
        hdf5::type datatype = ds.dataset_wrapper_.get_type();

        ::echelon::write(ds.dataset_wrapper_,datatype,mem_space,file_space,array);
    }
    
    template<typename T>
    friend void operator<<=(T& array,const dataset& ds)
    {
        std::vector<hsize_t> file_shape = ds.shape();

        hdf5::dataspace mem_space(file_shape);
        hdf5::dataspace file_space = ds.dataset_wrapper_.get_space();
        hdf5::type datatype = ds.dataset_wrapper_.get_type();

        ::echelon::read(ds.dataset_wrapper_,datatype,mem_space,file_space,array);
    }

    std::vector<hsize_t> shape()const;

    template<typename ...Args>
    slice slice(Args... args)const
    {
        std::vector<hsize_t> current_shape = shape();

        std::vector<range> boundaries;

        detail::calculate_slice_boundaries<0,Args...>::eval(current_shape,boundaries,args...);

        return ::echelon::slice(dataset_wrapper_,boundaries);
    }

    object_reference ref()const;

    hid_t id()const;
    const hdf5::dataset& get_native_handle()const;
private:
    hdf5::dataset dataset_wrapper_;

public:
    attribute_repository<dataset> attributes;
};



}

#endif
