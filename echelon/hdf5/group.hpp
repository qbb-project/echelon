#ifndef ECHELON_RAII_GROUP_HPP
#define ECHELON_RAII_GROUP_HPP

#include <string>
#include <hdf5.h>
#include <functional>

namespace echelon
{
namespace hdf5
{

class group
{
public:
    group();
    explicit group(hid_t group_id_);
    group(hid_t loc_id_, const std::string& name_, hid_t lcpl_id_,
          hid_t gcpl_id_, hid_t gapl_id_);
    group(hid_t loc_id_, const std::string& name_, hid_t gapl_id_);

    ~group();

    group(const group& other);
    group(group&& other);

    group& operator=(const group& other);
    group& operator=(group&& other);

    hsize_t iterate(hid_t group_id, H5_index_t index_type, H5_iter_order_t order,
                    hsize_t start_index, std::function<herr_t(hid_t,const char*)> op)const;

    hid_t id()const noexcept;
private:
    hid_t group_id_;
};

}
}

#endif
