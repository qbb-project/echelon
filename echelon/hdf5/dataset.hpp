#ifndef ECHELON_RAII_DATASET_HPP
#define ECHELON_RAII_DATASET_HPP

#include "type.hpp"
#include "dataspace.hpp"
#include "property_list.hpp"

#include <hdf5.h>
#include <string>

namespace echelon
{
namespace hdf5
{

class object_reference;

class dataset
{
public:
    explicit dataset(hid_t dataset_id_);
    dataset(hid_t loc_id, const std::string& name, const type& dtype,
            const dataspace& space, const property_list& lcpl,
            const property_list& dcpl,
            const property_list& dapl);
    dataset(hid_t loc_id, const std::string& name, const property_list& dapl);
    ~dataset();

    dataset(const dataset& other);
    dataset(dataset&& other);

    dataset& operator=(const dataset& other);
    dataset& operator=(dataset&&);


    void write(const type& mem_type, const dataspace& mem_space, const dataspace& file_space,
               const property_list& xfer_plist, const void* buf);
    void read(const type& mem_type, const dataspace& mem_space, const dataspace& file_space,
              const property_list& xfer_plist, void* buf)const;

    void set_extent(const std::vector<hsize_t>& dims);

    type get_type()const;
    dataspace get_space()const;

    hid_t id()const;
private:
    hid_t dataset_id_;
};

}
}

#endif
