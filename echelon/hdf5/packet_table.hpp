#ifndef ECHELON_RAII_PACKET_TABLE_HPP
#define ECHELON_RAII_PACKET_TABLE_HPP

#include <hdf5.h>
#include <hdf5_hl.h>

namespace echelon
{
namespace hdf5
{

class packet_table
{
public:
    packet_table(hid_t loc_id, const char* dset_name, hid_t dtype_id,
                 hsize_t chunk_size, int compression);
    packet_table(hid_t loc_id, const char* dset_name);

    ~packet_table();

    packet_table(const packet_table& other);
    packet_table(packet_table&& other);

    packet_table& operator=(const packet_table& other);
    packet_table& operator=(packet_table&& other);

    template<typename T>
    void append(const T& value)
    {
        H5PTappend(id(),1,&value);
    }

    template<typename T>
    T get(hsize_t index)
    {
        T value;

        H5PTread_packets(id(),index,1,&value);

        return value;
    }

    hid_t id()const;
private:
    hid_t table_id_;
};

}
}

#endif
