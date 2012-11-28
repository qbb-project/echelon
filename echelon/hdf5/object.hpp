#ifndef ECHELON_HDF5_OBJECT_HPP
#define ECHELON_HDF5_OBJECT_HPP

#include <hdf5.h>
#include <string>

namespace echelon
{

namespace hdf5
{

class dataset;
class group;

class object
{
public:
    object();

    explicit object(hid_t object_id_);
    object(hid_t loc_id_,const std::string& name);

    explicit object(const dataset& other);
    explicit object(const group& other);

    object(const object& other);
    object(object&& other);

    ~object();

    object& operator=(const object& other);
    object& operator=(object&& other);

    hid_t id()const;
private:
    hid_t object_id_;
};

bool exists(const object& loc,const std::string& name);

}

}

#endif
