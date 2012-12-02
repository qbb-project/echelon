//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_FILE_HPP
#define ECHELON_FILE_HPP

#include <echelon/group.hpp>
#include <echelon/hdf5/file.hpp>

#include <string>
#include <memory>

namespace echelon
{

class file
{
public:
    enum class create_mode
    {
        truncate,
        exclusive
    };

    enum class open_mode
    {
        read_only,
        read_write
    };

    //file interface

    file(const std::string& path, create_mode mode);
    file(const std::string& path, open_mode mode);

    /** returns the root group corresponding to this file
     *  \deprecated This function is deprecated in favor to file's group interface
     *              and will be removed in the near future.
     */
    group& root_group();

    //group interface

    group create_group(const std::string& name);

    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1);

    template<typename T>
    dataset create_dataset(const std::string& name,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1)
    {
        return root_group_.create_dataset<T>(name,dims,comp_level);
    }

    scalar_dataset create_scalar_dataset(const std::string& name, const type& datatype);

    template<typename T>
    scalar_dataset create_scalar_dataset(const std::string& name)
    {
        return root_group_.create_scalar_dataset<T>(name);
    }

    template<typename T>
    scalar_dataset create_scalar_dataset(const std::string& name,const T& value)
    {
        return root_group_.create_scalar_dataset(name,value);
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
        return root_group_.require_dataset<T>(name,dims,comp_level);
    }

    scalar_dataset require_scalar_dataset(const std::string& name, const type& datatype);

    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name)
    {
        return root_group_.require_scalar_dataset<T>(name);
    }

    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name,const T& value)
    {
        return root_group_.require_scalar_dataset(name,value);
    }

    void iterate(const std::function<void(const object&)>& op);

    object_reference ref()const;

    //general interface

    hid_t id() const noexcept;
private:
    hdf5::file file_wrapper_;
    group root_group_;
public:
    attribute_repository<group> attributes;
};

}

#endif
