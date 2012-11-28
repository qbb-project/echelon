#ifndef ECHELON_RAII_PROPERTY_LIST_HPP
#define ECHELON_RAII_PROPERTY_LIST_HPP

#include <hdf5.h>
#include <vector>

namespace echelon
{
namespace hdf5
{

class property_list_class
{
public:
    explicit property_list_class(hid_t class_id_);

    hid_t id() const;
    private:
    hid_t class_id_;
};

class property_list
{
public:
    explicit property_list(hid_t property_list_id_);
    explicit property_list(property_list_class cls_);
    ~property_list();

    property_list(const property_list& other);
    property_list(property_list&& other);

    property_list& operator=(const property_list& other);
    property_list& operator=(property_list&& other);

    void set_chunk(const std::vector<hsize_t>& dims);
    void set_deflate(unsigned int level);

    std::vector<hsize_t> get_chunk()const;

    hid_t id()const;
private:
    hid_t property_list_id_;
};

static const property_list default_property_list = property_list(H5P_DEFAULT);

}
}

#endif
