#ifndef ECHELON_GROUP_HPP
#define ECHELON_GROUP_HPP

#include <echelon/object.hpp>
#include <echelon/type.hpp>
#include <echelon/type_factory.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/creation_mode.hpp>
#include <echelon/attribute_repository.hpp>
#include <echelon/dataset.hpp>
#include <echelon/scalar_dataset.hpp>
#include <echelon/object_reference.hpp>
#include <echelon/utility.hpp>

#include <echelon/broken_contract_exception.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <exception>

namespace echelon
{

class file;

class non_existing_member_exception : public std::exception
{
public:
    non_existing_member_exception(const std::string& what_)
    :what_(what_)
    {}

    ~non_existing_member_exception() noexcept {}

    const char* what()const noexcept
    {
        return what_.c_str();
    }
private:
    std::string what_;
};

class group
{
public:
    friend class file;

    group create_group(const std::string& name);

    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1);

    template<typename T>
    dataset create_dataset(const std::string& name,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1)
    {
        return create_dataset(name,get_hdf5_type<T>(),dims,comp_level);
    }

    scalar_dataset create_scalar_dataset(const std::string& name, const type& datatype);

    template<typename T>
    scalar_dataset create_scalar_dataset(const std::string& name)
    {
        return create_scalar_dataset(name,get_hdf5_type<T>());
    }

    template<typename T>
    scalar_dataset create_scalar_dataset(const std::string& name,const T& value)
    {
        scalar_dataset ds = create_scalar_dataset<T>(name);

        ds <<= value;

        return ds;
    }

    object operator[](const std::string& name)const;

    group require_group(const std::string& name);

    dataset require_dataset(const std::string& name, const type& datatype,
                            const std::vector<hsize_t>& dims,
                            int comp_level = -1);

    template<typename T>
    dataset require_dataset(const std::string& name,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1)
    {
        return require_dataset(name,get_hdf5_type<T>(),dims,comp_level);
    }

    scalar_dataset require_scalar_dataset(const std::string& name, const type& datatype);

    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name)
    {
        return require_scalar_dataset(name,get_hdf5_type<T>());
    }

    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name,const T& value)
    {
        type datatype = get_hdf5_type<T>();

        if(exists(object(*this),name) && get_object_type_by_name(object(*this),name) == object_type::scalar_dataset)
        {
            scalar_dataset ds(hdf5::dataset(id(),name,hdf5::default_property_list));

            if(ds.datatype() != datatype)
                throw broken_contract_exception("The required datatype doesn't "
                                                "match the datatype of the dataset.");

            return ds;
        }
        else
        {
            scalar_dataset ds = create_scalar_dataset(name,datatype);

            ds <<= value;

            return ds;
        }
    }

    object_reference ref()const;

    hid_t id() const noexcept;
    const hdf5::group& get_native_handle()const;
private:
    friend class constructor_access;
    friend class object;

    explicit group(const file& loc, const std::string& name = "/");
    group(const object& parent, const std::string& name, creation_mode mode);
    explicit group(hdf5::group group_wrapper_);

    hdf5::group group_wrapper_;

public:
    attribute_repository<group> attributes;
};

}

#endif
