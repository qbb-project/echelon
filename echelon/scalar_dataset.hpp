 
#ifndef ECHELON_SCALAR_DATASET_HPP
#define ECHELON_SCALAR_DATASET_HPP

#include <echelon/object.hpp>

#include <echelon/type.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/property_list.hpp>

#include <echelon/object_reference.hpp>
#include <echelon/attribute_repository.hpp>

#include <echelon/data_transfer_broker.hpp>

namespace echelon
{

class scalar_dataset
{
public:
    scalar_dataset(const object& parent, const std::string& name,
                   const type& datatype);

    explicit scalar_dataset(hdf5::dataset dataset_wrapper_);

    template<typename T>
    friend inline void operator<<=(scalar_dataset& attr,const T& value)
    {
        write(attr.dataset_wrapper_,value);
    }

    template<typename T>
    friend inline void operator<<=(T& value,const scalar_dataset& attr)
    {
        read(attr.dataset_wrapper_,value);
    }

    object_reference ref()const;

    hid_t id()const;
    const hdf5::dataset& get_native_handle()const;
private:
    hdf5::dataset dataset_wrapper_;
public:
    attribute_repository<scalar_dataset> attributes;
};

}

#endif
