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
                     std::vector<std::tuple<hsize_t,hsize_t>>&)
    {
    }
};

template<std::size_t I,typename Front,typename ...Tail>
struct calculate_slice_boundaries<I,Front,Tail...>
{
    static void eval(const std::vector<hsize_t>& dims,
                     std::vector<std::tuple<hsize_t,hsize_t>>& boundaries,
                     Front front,
                     Tail... tail)
    {
        boundaries.push_back(get_boundaries(dims[I],front));

        calculate_slice_boundaries<I+1,Tail...>::eval(dims,boundaries,tail...);
    }
private:
    static std::tuple<hsize_t,hsize_t> get_boundaries(hsize_t dim,unbound_t)
    {
        return std::make_tuple(0,dim);
    }

    template<typename T>
    static std::tuple<hsize_t,hsize_t> get_boundaries(hsize_t dim,T value)
    {
        static_assert(std::is_integral<T>::value,
                      "only integral values are allowed in slicing expressions");

        assert(value < dim);

        return std::make_tuple(value,value + 1);
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
    void write(const T* data,const std::vector<hsize_t>& dims)
    {
        hdf5::dataspace mem_space(dims);
        hdf5::dataspace file_space = dataset_wrapper_.get_space();
        hdf5::type datatype = dataset_wrapper_.get_type();

        ::echelon::write(dataset_wrapper_,datatype,mem_space,file_space,data);
    }

    template<typename T>
    void read(T* data,const std::vector<hsize_t>& dims)const
    {
        hdf5::dataspace mem_space(dims);
        hdf5::dataspace file_space = dataset_wrapper_.get_space();
        hdf5::type datatype = dataset_wrapper_.get_type();

        ::echelon::read(dataset_wrapper_,datatype,mem_space,file_space,data);
    }

    template<typename T>
    typename std::enable_if< dataset_write_hook<T>::is_specialized , dataset& >::type
    operator=(const T& array)
    {
        auto dims = dataset_write_hook<T>::dims(array);

        std::vector<hsize_t> mem_dims(begin(dims), end(dims));

        write(dataset_write_hook<T>::data(array),mem_dims);

        return *this;
    }
    
    template<typename T,
             typename std::enable_if< dataset_read_hook<T>::is_specialized , int>::type = 0 >
    operator T()const
    {
        std::vector<hsize_t> simple_extend_dims =
            dataset_wrapper_.get_space().get_simple_extent_dims();

        std::vector<std::size_t> dims(begin(simple_extend_dims),end(simple_extend_dims));

        T result = dataset_read_hook<T>::create(dims);

        read(dataset_read_hook<T>::data(result),simple_extend_dims);

        return result;
    }

    template<typename ...Args>
    slice slice(Args... args)const
    {
        std::vector<hsize_t> dims =
                    dataset_wrapper_.get_space().get_simple_extent_dims();

        std::vector<std::tuple<hsize_t,hsize_t>> boundaries;

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
