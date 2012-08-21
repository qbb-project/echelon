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
    static void eval(const std::vector<hsize_t>& dims,
                     std::vector<range>& boundaries,
                     Front front,
                     Tail... tail)
    {
        boundaries.push_back(get_boundaries(dims[I],front));

        calculate_slice_boundaries<I+1,Tail...>::eval(dims,boundaries,tail...);
    }
private:
    static range get_boundaries(hsize_t dim,unbound_t)
    {
        return range(0,dim);
    }

    static range get_boundaries(hsize_t,range r)
    {
        return r;
    }

    template<typename T>
    static range get_boundaries(hsize_t dim,T value)
    {
        static_assert(std::is_integral<T>::value,
                      "only integral values are allowed in slicing expressions");

        assert(value < dim);

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
        auto current_dims = dims(array);

        std::vector<hsize_t> mem_dims(begin(current_dims), end(current_dims));

        hdf5::dataspace mem_space(mem_dims);
        hdf5::dataspace file_space = ds.dataset_wrapper_.get_space();
        hdf5::type datatype = ds.dataset_wrapper_.get_type();

        ::echelon::write(ds.dataset_wrapper_,datatype,mem_space,file_space,array);
    }
    
    template<typename T>
    friend void operator<<=(T& array,const dataset& ds)
    {
        std::vector<hsize_t> simple_extend_dims =
            ds.dataset_wrapper_.get_space().get_simple_extent_dims();

        std::vector<std::size_t> dims(begin(simple_extend_dims),end(simple_extend_dims));

        hdf5::dataspace mem_space(simple_extend_dims);
        hdf5::dataspace file_space = ds.dataset_wrapper_.get_space();
        hdf5::type datatype = ds.dataset_wrapper_.get_type();

        ::echelon::read(ds.dataset_wrapper_,datatype,mem_space,file_space,array);
    }

    template<typename ...Args>
    slice slice(Args... args)const
    {
        std::vector<hsize_t> dims =
                    dataset_wrapper_.get_space().get_simple_extent_dims();

        std::vector<range> boundaries;

        detail::calculate_slice_boundaries<0,Args...>::eval(dims,boundaries,args...);

        return ::echelon::slice(dataset_wrapper_,boundaries);
    }

    object_reference ref()const;

    virtual hid_t id()const;
private:
    hdf5::dataset dataset_wrapper_;

public:
    attribute_repository<dataset> attributes;
};



}

#endif
