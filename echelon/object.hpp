#ifndef ECHELON_OBJECT_HPP
#define ECHELON_OBJECT_HPP

#include <hdf5.h>
#include <exception>
#include <string>

namespace echelon
{

class wrong_object_type_exception : public std::exception
{
public:
    wrong_object_type_exception(const std::string& what_)
    :what_(what_)
    {}

    ~wrong_object_type_exception() noexcept {}

    const char* what()const noexcept
    {
        return what_.c_str();
    }
private:
    std::string what_;
};

class group;
class dataset;
class scalar_dataset;

class object_reference;

class object
{
public:
    //fast workaround!
    //only needed for reading datasets of object_references by now;
    //should not be used for anything different and should be
    //deleted after we fixed this problem
    object();

    explicit object(hid_t object_id_);
    object(hid_t loc_id_,const std::string& name);

    object(const object& other);
    object(object&& other);

    explicit object(const group& object_);
    explicit object(const dataset& object_);
    explicit object(const scalar_dataset& object_);

    ~object();

    object& operator=(const object& other);
    object& operator=(object&& other);

    object& operator=(const group& object_);
    object& operator=(const dataset& object_);
    object& operator=(const scalar_dataset& object_);

    operator group()const;
    operator dataset()const;
    operator scalar_dataset()const;

    object_reference ref()const;

    hid_t id()const;
private:
    hid_t object_id_;
};



}

#endif
