 
#ifndef ECHELON_SCALAR_DATASET_HPP
#define ECHELON_SCALAR_DATASET_HPP

#include <echelon/object.hpp>

#include <echelon/type.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/property_list.hpp>

namespace echelon
{

class scalar_dataset
{
public:
    scalar_dataset(const object& parent, const std::string& name,
                   const type& datatype, const std::vector<hsize_t>& dims);

    virtual ~scalar_dataset() noexcept
    {}

    template<typename T>
    scalar_dataset& operator=(const T& value)
    {
        hdf5::dataspace mem_space;

        hdf5::dataspace file_space = dataset_wrapper_.get_space();

        dataset_wrapper_.write(dataset_wrapper_.get_type(),
                               mem_space,file_space,
                               hdf5::default_property_list,&value);

        return *this;
    }

    template<typename T>
    operator T()const
    {
        T value;

        hdf5::dataspace mem_space;

        hdf5::dataspace file_space = dataset_wrapper_.get_space();

        dataset_wrapper_.read(dataset_wrapper_.get_type(),
                              mem_space,file_space,
                              hdf5::default_property_list,&value);

        return value;
    }

    virtual hid_t id()const;
private:
    hdf5::dataset dataset_wrapper_;
};

}

#endif
