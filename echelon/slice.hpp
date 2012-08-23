#ifndef ECHELON_SLICE_HPP
#define ECHELON_SLICE_HPP

#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/data_transfer_broker.hpp>

#include <echelon/range.hpp>

#include <cassert>
#include <vector>
#include <tuple>

namespace echelon
{

template<typename T>
inline std::vector<std::size_t> dims(const std::vector<T>& container)
{
    return { container.size() };
}

template<typename C>
inline std::vector<std::size_t> dims(const C& container)
{
    return container.dims();
}

class slice
{
public:
    slice(hdf5::dataset sliced_dataset_,
          const std::vector<range>& ranges);

    template<typename T>
    void operator<<=(const T& array)
    {
        auto current_dims = dims(array);

        std::vector<hsize_t> mem_dims(begin(current_dims), end(current_dims));

        hdf5::dataspace mem_space(mem_dims);
        hdf5::dataspace file_space = selected_dataspace_;
        hdf5::type datatype = sliced_dataset_.get_type();

        ::echelon::write(sliced_dataset_,datatype,mem_space,file_space,array);
    }

    template<typename T>
    friend void operator<<=(T& array,const slice& sl)
    {
        std::vector<hsize_t> slice_dims = sl.size();

        std::vector<std::size_t> dims(begin(slice_dims),end(slice_dims));

        hdf5::dataspace mem_space(slice_dims);
        hdf5::dataspace file_space = sl.selected_dataspace_;
        hdf5::type datatype = sl.sliced_dataset_.get_type();

        ::echelon::read(sl.sliced_dataset_,datatype,mem_space,file_space,array);
    }

    const std::vector<hsize_t>& size()const;
private:
    hdf5::dataset sliced_dataset_;
    hdf5::dataspace selected_dataspace_;

    std::vector<hsize_t> size_;
};

};

#endif
