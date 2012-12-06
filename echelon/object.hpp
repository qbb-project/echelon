//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_OBJECT_HPP
#define ECHELON_OBJECT_HPP

#include <echelon/hdf5/object.hpp>
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
    explicit object(const hdf5::object& object_wrapper_);

    explicit object(const group& object_);
    explicit object(const dataset& object_);
    explicit object(const scalar_dataset& object_);

    object& operator=(const group& object_);
    object& operator=(const dataset& object_);
    object& operator=(const scalar_dataset& object_);

    operator group()const;
    operator dataset()const;
    operator scalar_dataset()const;

    object_reference ref()const;

    hid_t id()const;
    const hdf5::object& get_native_handle()const;
private:
    hdf5::object object_wrapper_;
};



}

#endif
