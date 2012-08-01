#ifndef ECHELON_SLICE_HPP
#define ECHELON_SLICE_HPP

#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/data_transfer_broker.hpp>

#include <cassert>
#include <vector>
#include <tuple>

namespace echelon
{

class slice
{
public:
    slice(hdf5::dataset sliced_dataset_,
          const std::vector<std::tuple<hsize_t,hsize_t>>& boundaries);

    template<typename T>
    void write(const T* data,const std::vector<hsize_t>& dims)
    {
        hdf5::dataspace mem_space(dims);
        hdf5::dataspace file_space = selected_dataspace_;
        hdf5::type datatype = sliced_dataset_.get_type();

        ::echelon::write(sliced_dataset_,datatype,mem_space,file_space,data);
    }

    template<typename T>
    void read(T* data,const std::vector<hsize_t>& dims)const
    {
        hdf5::dataspace mem_space(dims);
        hdf5::dataspace file_space = selected_dataspace_;
        hdf5::type datatype = sliced_dataset_.get_type();

        ::echelon::read(sliced_dataset_,datatype,mem_space,file_space,data);
    }

    template<typename T>
    typename std::enable_if< dataset_write_hook<T>::is_specialized , slice& >::type
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
        std::vector<hdf5::hyperslab_block> hyperslab_blocks =
                   selected_dataspace_.get_select_hyperslab_blocks();

        const std::size_t num_of_hyperslab_block = hyperslab_blocks.size();

        assert(num_of_hyperslab_block == 1);

        std::vector<hsize_t> block_dims = hyperslab_blocks[0].dims();

        std::vector<std::size_t> dims(begin(block_dims),end(block_dims));

        T result = dataset_read_hook<T>::create(dims);

        read(dataset_read_hook<T>::data(result),block_dims);

        return result;
    }

private:
    hdf5::dataset sliced_dataset_;
    hdf5::dataspace selected_dataspace_;
};

};

#endif
